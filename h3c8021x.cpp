/*
    Copyright (C) 2010 lovewilliam <lovewilliam@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "h3c8021x.h"
#include "md5_ctx.h"

#include <iostream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <netinet/ether.h>
#include <netinet/in.h>

#define FALSE false
#define TRUE true

#define LOWORD(i)((WORD)(i))  
#define HIWORD(i)((WORD)(((DWORD)(i)>>16)&0xffff)) 

#define LOBYTE(i)((BYTE)(i))  
#define HIBYTE(i)((BYTE)(((WORD)(i)>>8)&0xff)) 

#define _DEBUG_

using namespace std;

h3c8021x::h3c8021x()
{
    count=0x00;
    run = FALSE;
    allDevs = NULL;
    log = NULL;
    signalGotData = NULL;
    handler = NULL;
    u_char t[6] = {0x01,0x80,0xc2,0x00,0x00,0x03};
    memcpy(multiCastMacAddr,t,6);
    u_char m[6]={0x00,0x11,0x09,0xfb,0x14,0x7d};
    memcpy(switchMac,m,6);

    //printf("%x:%x:%x:%x:%x:%x\n",m[0],m[1],m[2],m[3],m[4],m[5]);
    char cver[14] ={ 'C', 'H', ' ', 'V', '2', '.', '4', '0', '-', '0', '3', '2', '6', 0 };
    memcpy(clientVersion, cver, 14);
    srand(time(NULL));
}

h3c8021x::~h3c8021x()
{

}

void h3c8021x::setRun(bool r)
{
    run =r;
}

bool h3c8021x::getRun()
{
	return run;
}

void h3c8021x::setDHCPcmd(string s)
{
    dhcpcmd = s;
}


void h3c8021x::setLog(string* l)
{
    log = l;
}

void h3c8021x::setUserName(string u)
{
    username = u;
}


void h3c8021x::setPassword(string p)
{
    password = p;
}

void h3c8021x::setSignal(int* s)
{
    signalGotData = s;
    *signalGotData = 0;
}


int h3c8021x::getSignal()
{
    return *signalGotData;
}

void h3c8021x::setDefaultNic(string d)
{
    defaultDev = d;
}

void h3c8021x::message(const char* msg)
{
    time_t timep;
    time(&timep);
    string acctime = asctime(gmtime(&timep));
    acctime.erase(acctime.length()-1);
    cout <<'['<<acctime<<"] "<<msg << endl;
    if(log!=NULL)
    {
	(*log) += '\n';
	(*log) += '[';
	(*log).append(acctime);
	(*log) += ']';
	(*log) += ' ';
	(*log).append(msg);
    }
}
void h3c8021x::message(string msg)
{
    message(msg.c_str());
}

pcap_if_t* h3c8021x::getAllNic()
{
    if(pcap_findalldevs(&allDevs,errbuf)==-1)
    {
	message(errbuf);
	return NULL;
    }
    return allDevs;
}

string h3c8021x::getMacAddr(string nic)
{
    int sockfd;
    struct ifreq struReq;
    sockfd = socket(PF_INET,SOCK_STREAM,0);
    memset(&struReq,0,sizeof(struReq));
    strncpy(struReq.ifr_name, nic.c_str(), sizeof(struReq.ifr_name));
    ioctl(sockfd,SIOCGIFHWADDR,&struReq);
    macaddr = (ether_ntoa((ether_addr*)(struReq.ifr_hwaddr.sa_data)));
    string msgstr = defaultDev;
    msgstr.append(" MAC:");
    msgstr.append(macaddr);
    message(msgstr);
    int count = 0;
    string temp = "0";
    for(int i =0;i<macaddr.length();i++)
    {
	if(macaddr.at(i)!=':')
	{
	    char a = macaddr.at(i);
	    temp.append(string(1,a));
	}else
	{
	    nicMac[count] = char(strtol(temp.c_str(),NULL,16));
	    count++;
	    temp.clear();
	}
    }
    nicMac[count] = char(strtol(temp.c_str(),NULL,16));
    close(sockfd);
    return macaddr;
}


bool h3c8021x::openNic()
{
    handler = pcap_open_live(defaultDev.c_str(), 256, 0, 10, errbuf);
    if(handler==NULL)
	
    {
	message(string("*Can't open device").append(defaultDev).append(" Error:").append(errbuf));
	return FALSE;
    }
    getMacAddr(defaultDev.c_str());
    filter = "ether dst "+macaddr+" and ether proto 0x888e";
    cout<<"Filter:"<<filter<<endl;
    if(pcap_compile(handler, &fp, filter.c_str(),0,0)==-1)
    {
	return FALSE;
    }
    if(pcap_setfilter(handler,&fp)==-1)
    {
	return FALSE;
    }
    return TRUE;
}

bool h3c8021x::closeNic()
{
    if(handler!=NULL)
    {
	pcap_close(handler);
	handler==NULL;
    }
    return TRUE;
}

bool h3c8021x::login()
{
    if(username.length()==0)
    {
	message("Empty Username!");
	return FALSE;
    }
    if(password.length()==0)
    {
	message("Empty Password");
	return FALSE;
    }
    if(defaultDev.length()==0)
    {
	message("*Default NIC Not set Using System Default NIC");
    }
    if(allDevs==NULL)
    {
	getAllNic();
    }
    if(allDevs==NULL)
    {
	message("*Have no premission to access nic");
	return FALSE;
    }
    defaultDev = allDevs->name;
    if(!openNic())
    {
	message("*Error while opening NIC "+defaultDev);
	return FALSE;
    }
    if(!sendEAPOLStart())
    {
	return FALSE;
    }
    //switch(pcap_loop(handler,-1,dealPacket,NULL))
    int result;
    struct pcap_pkthdr *header;
    const u_char *inpacket;
    time_t tickn,ticko;
    tickn = ticko = clock();
    while(run && (result = pcap_next_ex(handler,&header,&inpacket))>=0)
    {
    	//cout<<"running in loop"<<endl;
	//result = pcap_next_ex(handler,&header,&inpacket);
	//cout<<"Result="<<result<<endl;
	switch(result)
	{
	    case(-2):
	    {
		message("*Loop Break! Returning..");
		closeNic();
		return -2;
	    }
	    case(-1):
	    {
		message("*An error occured while reading the packet!");
		closeNic();
		return -1;
	    }
	    case(0):
	    {
		tickn = clock();
		if (tickn - ticko >= 90000)//90000ms
		{
		    message("*Timeout expired!");
		}
		ticko = clock();
		continue;
	    }
	    case(1):
	    {
	    	cout<<"Got one packet!"<<endl;
		if(signalGotData!=NULL)
		{
		    *signalGotData = strlen((char*)inpacket);
		}
		if(!dealPacket(NULL,header,inpacket))
		{
		    closeNic();
		    run = FALSE;
		    return FALSE;
		}
	    }
	}
    }
}

bool h3c8021x::logoff()
{
//    run = FALSE;
    if(!run)
    	return TRUE;
    if(handler!=NULL)
    {
	sendEAPOLLogoff();
	run=FALSE;
	handler = NULL;
    }
    return TRUE;
}

bool h3c8021x::keepOnLine()
{

}

////////////////////////////////////////////////////////////////////////////
void h3c8021x::testSuit()
{
    cout<<"Start Testing"<<endl;
    cout<<"Initializing NIC "<<defaultDev;
    if(allDevs==NULL)
    {
	getAllNic();
    }
    defaultDev = allDevs->name;
    if(!openNic())
    {
	message("Error while opening NIC "+defaultDev);
	return;
    }
    cout<<" Done!"<<endl;
    //
    sendEAPOLStart();
    cout<<"sent EAPOL Start"<<endl;
    sendVersionAct();
    cout<<"sent Version Act"<<endl;
    sendEAPResponseUsername();
    cout<<"sent EAPResponse Username"<<endl;
    u_char c[] = "0123456789abcdef";
    sendEAPResponseMD5Challenge(c);
    cout<<"sent EAPResponse Password"<<endl;
}


//////////////////////////////////////////////////////////////////////////// 
//Handle Packet
//
////////////////////////////////////////////////////////////////////////////
void h3c8021x::fillBuffer(u_char* buf)
{
    memcpy(buf,switchMac,6);
    memcpy(buf+6,nicMac,6);
    u_char et[2] = {0x88,0x8e};
    memcpy(buf+12,et,2);
    u_char v = 0x01;
    memcpy(buf+14,&v,1);
}



//PACKET Structure
//0x00-0x05 TARGET MAC
//0x06-0x0b SOURCE MAC
//0x0c-0x0d PROTOCOL
//0x0e VERSION
//0x0f TYPE			<<=
//0x10-0x11 PACKET LEN
//0x12 Code			<<=Identify this #1
//0x13 Count Identifier
//0x14-0x15 PACKET LEN
//0x16 EAP TYPE			<<=Identify this #2

bool h3c8021x::sendEAPOLStart()
{
    //60byte Total
    //0x00-0x05（01 80 c2 00 00 03）Multicast Mac Addr
    //0x06-0x0b（00 e0 4c 30 35 6a）Nic Mac
    //0x0c-0x0d（88 8e）PAE Ethernet
    //0x0e（01）Version
    //0x0f (01) Packet Type
    u_char buf[100] = { 0 };
    memcpy(buf,multiCastMacAddr,6);
    memcpy(buf+6,nicMac,6);
    //printf("NICMAC:%x:%x:%x:%x:%x:%x\n",nicMac[0],nicMac[1],nicMac[2],nicMac[3],nicMac[4],nicMac[5]);
    buf[12]=0x88;
    buf[13]=0x8e;
    buf[14]=0x01;
    buf[15]=EAPOL_START;
    if(pcap_sendpacket(handler,buf,60)==-1)
    {
	message("*Failed to send login packet.");
	return FALSE;
    }
    message("*Login Sent!");
    for(int i=0;i<100;i++)
    {
    	printf("%x ",buf[i]);
	if(i%16==0&&i!=0)
	{
		cout<<endl;
	}
    }
    return TRUE;
}

bool h3c8021x::sendVersionAct()
{
    //60 byte
    u_char buf[100] = { 0 };
    VersionActPacket *vap = (VersionActPacket*)buf;
    fillBuffer(buf);
    vap->base.PacketType=EAP;
    vap->base.Len1=htons(0x31);
    vap->base.Code=EAP_RESPONSE;
    vap->base.Id=0x01;
    vap->base.Len2=vap->base.Len1;
    vap->base.EapType=EAP_NOTIFICATION;
    GenerateVersion(vap->version);
    if(pcap_sendpacket(handler,buf,67)==-1)
    {
	message("*Failed to send VersionAct packet.");
	return FALSE;
    }
    return TRUE;
}

bool h3c8021x::sendEAPResponseUsername()
{
    //60 byte
    u_char buf[60] = { 0 };
    UsernamePacket *up = (UsernamePacket*)buf;
    fillBuffer(buf);
    up->base.PacketType=EAP;
    up->base.Code=EAP_RESPONSE;
    up->base.Id= count;
    up->base.EapType=EAP_IDENTIFY;
    u_char u[] = {0x15,0x04};
    memcpy(up->Unknown,u,2);
    up->base.Len1 = htons(username.length() + 0x0b);
    up->base.Len2 = up->base.Len1;
    bpf_u_int32 ip=0x00;/* The IP of our sniffing device */
    pcap_if_t *p = allDevs;
    while(ip==0&&p)
    {
	if(*(p->name) == *(defaultDev.c_str()))
	{
	    pcap_addr_t* ap = p->addresses;
	    for(ap;ap!=NULL;ap = ap->next)
	    {
		if(ap->addr->sa_family==AF_INET)
		{
		    ip = ((sockaddr_in*)ap->addr)->sin_addr.s_addr;
		    break;
		}
	    }
	}
	p = p->next;
    }
    nicIP[3] = (ip&0xFF000000)>>24;
    nicIP[2] = (ip&0x00FF0000)>>16;
    nicIP[1] = (ip&0x0000FF00)>>8;
    nicIP[0] = (ip&0x000000FF)>>0;
    char pb[100]={0};
    string temp = defaultDev;
    sprintf(pb,": IP Address is %d.%d.%d.%d",nicIP[0],nicIP[1],nicIP[2],nicIP[3]);
    temp.append(pb);
    message(temp.c_str());
    memcpy(up->IP,&ip,4);
    memcpy(up->Username,username.c_str(),username.length());
    if(pcap_sendpacket(handler,buf,60)==-1)
    {
	message("*Failed to send EAPResponseUsername packet.");
	return FALSE;
    }
    return TRUE;
}


bool h3c8021x::sendEAPOLLogoff()
{
    //60byte
    u_char buf[60]={0};
    fillBuffer(buf);
    PacketBase *lop = (PacketBase*)buf;
    buf[15] = EAP_NOTIFICATION;
    if(pcap_sendpacket(handler,buf,60)==-1)
    {
	message("*Failed to send EAPLogoff packet.");
	return FALSE;
    }
    message("*Logout sent");
    return TRUE;
}

bool h3c8021x::sendEAPResponseMD5Challenge(u_char *chap)//Password
{
    //60byte
    u_char buf[100] = {0};
    PasswordPacket *pp = (PasswordPacket*)buf;
    fillBuffer(buf);
    pp->base.PacketType = EAP;
    pp->base.Code = EAP_RESPONSE;
    pp->base.EapType = EAP_MD5;
    pp->base.Id = count;
    pp->base.Len1 = htons(username.length()+0x16);
    pp->base.Len2 = pp->base.Len1;
    pp->EALen = 0x10;
    //Calculate MD5 Digest
    /*Buffer Struct
     *
     *ID
     *Password
     *Chap
     */
    u_char temp[1+64+16];
    MD5_CTX md5T;
    u_char digest[16];
    temp[0] = count;
    memcpy(temp+0x01,password.c_str(),password.length());
    memcpy(temp+0x01+password.length(),chap,16);
    md5T.MD5Update(temp,17+password.length());
    md5T.MD5Final(digest);
    memcpy(pp->MD5Password,digest,16);
    memcpy(pp->Username,username.c_str(),username.length());
    if(pcap_sendpacket(handler,buf,60)==-1)
    {
	message("*Failed to send EAPResponseMD5Challenge packet.");
	return FALSE;
    }
    return TRUE;
}

bool h3c8021x::sendKeepOnLine()
{
    //60byte
    u_char buf[60] = {0};
    KeeponlinePacket *kp = (KeeponlinePacket*)buf;
    fillBuffer(buf);
    
}

bool h3c8021x::dealPacket(u_char* args,const pcap_pkthdr* header, const u_char* packet)
{
    PacketBase *dp = (PacketBase*)packet;
    count = dp->Id;
    memcpy(switchMac,dp->SourMAC,6);

    //printf("Switch MAC:%x:%x:%x:%x:%x:%x\n",switchMac[0],switchMac[1],switchMac[2],switchMac[3],switchMac[4],switchMac[5]);

    string tmac = "TargetMac :";
    char tmacs[32]={0};
    sprintf(tmacs,"%x:%x:%x:%x:%x:%x",switchMac[0],switchMac[1],switchMac[2],switchMac[3],switchMac[4],switchMac[5]);
    string temp = tmacs;
    tmac.append(temp);
    message(tmac);
    
    switch(dp->Code)
    {
	case(EAP_REQUEST):
	{
	    switch(dp->EapType)
	    {
		case(EAP_IDENTIFY):
		{
		    message("*EAP_IDENTIFY Recieved");
		    message("*Send Username");
		    //Send Username
		    if(!sendEAPResponseUsername())
		    {
			message("*EAP Username Response Failed");
			return FALSE;
		    }
		    break;
		}
		case(EAP_NOTIFICATION):
		{
		    message("*EAP_NOTIFICATION Received");
		    message("*Send Version");
		    if(!sendVersionAct())
		    {
			message("*Version ACT Failed!");
			return FALSE;
		    }
		    break;
		}
		case(EAP_MD5):
		{
		    message("*EAP_MD5 Received.");
		    message("*Send MD5 Password");
		    //send password
		    MD5Challenge* mc = (MD5Challenge*)packet;
		    if(!sendEAPResponseMD5Challenge(mc->chap))
		    {
			message("*Send EAPResponse MD5Challenge Failed");
			return FALSE;
		    }
		    break;
		}
		case(EAP_KEEPONLINE):
		{
		    //Send Keeponline
		    if(!sendKeepOnLine())
		    {
			message("*send Keep Online Failed!");
			return FALSE;
		    }
		    break;
		}
		default:
		    break;
	    }
	    break;
	}
	case(EAP_SUCCESS):
	{
	    message("*Login Success!");
	    system(dhcpcmd.c_str());
	    break;
	}
	case(EAP_FAILURE):
	{
	    if(dp->EapType==EAP_LOGOUT)
	    {
		message("*Logged out!");
		//FIXME!!!
		//return closeNic();
	    }else
	    {
		message((const char *) (packet + 0x18));
	    }
	    return FALSE;
	    break;
	}
	case(EAP_OTHER):
	{
	    //Get Token
	    //Will be used in the KeepOnlinePacket
	    getToken(packet);
	    break;
	}
    }
    return TRUE;
}

bool h3c8021x::getToken(const u_char* raw_token)
{
    TokenPacket* atoken = (TokenPacket*) raw_token;
    //Identifier should be 0x23 0x44 0x23 0x31
    if(atoken->Identifier[0]==0x23&&
	atoken->Identifier[1]==0x44&&
	atoken->Identifier[2]==0x23&&
	atoken->Identifier[3]==0x31)
    {
	memcpy(token,atoken->Token,33);
	GenerateMagic();
	return TRUE;
    }
    return FALSE;
}



void h3c8021x::GenerateVersion(u_char* buf)
{
    buf[0] = 0x01;
    buf[1] = 0x16;
    
    unsigned long magic = rand();
    //initial strMagic, store the of string of the hexadecimal value of magic in it*/
    char strMagic[9] = { 0 };
    unsigned char strTemp[4] = { 0 };
    memcpy(strTemp, (unsigned char *) &magic, 4);
    sprintf(strMagic, "%02x%02x%02x%02x", strTemp[0], strTemp[1],
	    strTemp[2], strTemp[3]);
	    unsigned char version[20];
	    memset(version, 0, sizeof(version));
	    memcpy(version, clientVersion, strlen((char*)clientVersion));
	    memcpy(version + 16, (unsigned char *) &magic, 4);
	    
	    //set the variable 20 bytes of the information about version 
	    EncodeVersion(strMagic, version, 0x10);
	    char v[] = "HuaWei3COM1X";
	    EncodeVersion(v, version, 0x14);
	    memcpy(buf + 2, version, 20);
	    buf[22] = 0x02;
	    buf[23] = 0x16;
	    //the last 20 bytes
	    char winVersion[20] = { 
		0x3a, 0x71, 0x38, 0x01, 0x0b, 0x3b, 0x7e, 0x3d,
		0x26, 0x7c, 0x7c, 0x17, 0x0b, 0x46, 0x08, 0x32,
		0x32, 0x08, 0x46, 0x0b };
		memcpy(buf + 24, winVersion, 20);
		
}

//Written By AGanNo2 (AGanNo2@163.com)
// use strConst to encrypt strDest
void h3c8021x::EncodeVersion(char* strConst, unsigned char* strDest, int iSize)
{
    char *temp = new char[iSize];
    
    int iTimes = iSize / strlen(strConst);
    
    for (int i = 0; i < iTimes; i++)
	memcpy(temp + i * strlen(strConst), strConst,
	       strlen(strConst));
	       
	       memcpy(temp + iTimes * strlen(strConst), strConst,
		      iSize % strlen(strConst));
		      
		      for (int i = 0; i < iSize; i++)
			  strDest[i] = strDest[i] ^ temp[i];
		      
		      for (int i = 0; i < iSize; i++)
			  strDest[iSize - i - 1] = strDest[iSize - i - 1] ^ temp[i];
		      
		      delete[]temp;
		      
}

void h3c8021x::GenerateMagic()
{
    for(int i=0;i<4;i++)
    {
	CalcASC(token + 8 * i);
    }
    MD5_CTX md5T;
    md5T.MD5Update(token,32);
    md5T.MD5Final(token);
    token[16] = 0;
    md5T.MD5Update(token,16);
    md5T.MD5Final(token + 16);
    
}

void h3c8021x::CalcASC(u_char* buf)
{
    //H3C的算法，所得结果为ASCII字符串
    WORD Res;
    DWORD dEBX,dEBP;
    DWORD dEDX = 0x10;
    DWORD mSalt[4] = {0x56657824,0x56745632,0x97809879,0x65767878};
    
    DWORD dECX = *((DWORD*)buf);
    DWORD dEAX = *((DWORD*)(buf + 4));
    
    dEDX *= 0x9E3779B9;
    
    while(dEDX != 0)
    {
	dEBX = dEBP = dECX;
	dEBX >>= 5;
	dEBP <<= 4;
	dEBX ^= dEBP;
	dEBP = dEDX;
	dEBP >>= 0x0B;
	dEBP &= 3;
	dEBX += mSalt[dEBP];
	dEBP = dEDX;
	dEBP ^= dECX;
	dEDX += 0x61C88647;
	dEBX += dEBP;
	dEAX -= dEBX;
	dEBX = dEAX;
	dEBP = dEAX;
	dEBX >>= 5;
	dEBP <<= 4;
	dEBX ^= dEBP;
	dEBP = dEDX;
	dEBP &= 3;
	dEBX += mSalt[dEBP];
	dEBP = dEDX;
	dEBP ^= dEAX;
	dEBX += dEBP;
	dECX -= dEBX;
    }
    
    
    Res = LOWORD(dECX);
    *buf = LOBYTE(Res);
    *(buf+1) = HIBYTE(Res);
    
    Res = HIWORD(dECX);
    *(buf+2) = LOBYTE(Res);
    *(buf+3) = HIBYTE(Res);
    
    Res = LOWORD(dEAX);
    *(buf+4) = LOBYTE(Res);
    *(buf+5) = HIBYTE(Res);
    
    Res = HIWORD(dEAX);
    *(buf+6) = LOBYTE(Res);
    *(buf+7) = HIBYTE(Res);
}

