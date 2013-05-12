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
#include "8021xframe.h"

#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <iostream>

#define TRUE true
#define FALSE false

using namespace std;

char* progname;

void usage();
void term();
void listDev();
void runAsDomain();
void checkDevExists(char*);

int main(int argc,char** argv)
{
    progname = argv[0];
    char* nic = NULL;
    char* username = NULL;
    char* password = NULL;
    bool daemon = FALSE;
    bool rundhcp = FALSE;
    const char* const soptions = "iuprdtlh:";
    const struct option loptions[] ={
	{"nic",1,NULL,'i'},
	{"username",1,NULL,'u'},
	{"password",1,NULL,'p'},
	{"rundhcp",0,NULL,'r'},
	{"daemon",0,NULL,'d'},
	{"terminate",0,NULL,'t'},
	{"list",0,NULL,'l'},
	{"help",0,NULL,'h'},
	{NULL,0,NULL,0}
    };

	int next;
	do
	{
	    next = getopt_long(argc,argv,soptions,loptions,NULL);
	    switch(next)
	    {
		case 'i':
		{
		    optarg = argv[optind];
		    if(optarg==NULL)
			break;
		    nic = (char*)malloc(strlen(optarg)+1);
		    strcpy((char*)nic,optarg);
		    break;
		}
		case 'u':
		{
		    optarg = argv[optind];
		    if(optarg==NULL)
			break;
		    username = (char*)malloc(strlen(optarg)+1);
		    strcpy(username,optarg);
		    break;
		}
		case 'p':
		{
		    optarg = argv[optind];
		    if(optarg==NULL)
			break;
		    password = (char*)malloc(strlen(optarg)+1);
		    strcpy((char*)password,optarg);
		    break;
		}
		case 'd':
		{
		    daemon = TRUE;
		    break;
		}
		case 'r':
		{
		    rundhcp = TRUE;
		    break;
		}
		case 't':
		{
		    term();
		    exit(0);
		    break;
		}
		case 'l':
		{
		    listDev();
		    exit(0);
		    break;
		}
		case 'h':
		{
		    usage();
		    exit(0);
		    break;
		}
		case '?':
		{
		    usage();
		    exit(0);
		    break;
		}
		case -1:
		{
		    //usage();
		    break;
		}
		default:
		    break;
	    }
	}while(next!=-1);
	if(nic==NULL)
	{
	    cout<<"Require a NIC"<<endl;
	    //usage();
	    exit(0);
	}
	if(username==NULL)
	{
	    cout<<"Require a Username"<<endl;
	    //usage();
	    exit(0);
	}
	if(password==NULL)
	{
	    cout<<"Require a Password"<<endl;
	    //usage();
	    exit(0);
	}
	if(daemon)
	{
	    runAsDomain();
	}
	//cout<<"nic="<<nic<<endl;
	//cout<<"u="<<username<<endl;
	//cout<<"p="<<password<<endl;
	checkDevExists(nic);
	h3c8021x dot1x;
	dot1x.setDefaultNic(nic);
	dot1x.setUserName(username);
	dot1x.setPassword(password);
	dot1x.setRun(TRUE);
	if(rundhcp)
	{
	    string temp = "dhcpcd ";
	    temp.append(nic);
	    dot1x.setDHCPcmd(temp);
	}
	//dot1x.testSuit();
	//dot1x.login();
	dot1x.logoff();
    return 0;
}

void usage()
{
    printf("H3C 802.1x Client\n"
	    "usage:%s  -i nic -u username -p password [-d] [-t] [-l] [-h]\n"
	    "\t\t-i --nic\tNetwork Interface Card.\n"
	    "\t\t-u --username\tUsername.\n"
	    "\t\t-p --password\tPassword.\n"
	    "\t\t-r --rundhcp\tRun DHCP after login\n"
	    "\t\t-d --daemon\tRun as daemon.\n"
	    "\t\t-t --terminate\tTerminate.\n"
	    "\t\t-l --list\tList all available Nic.\n"
	    "\t\t-h --help\tDiaplay usage.\n"
	    "Report bug to lovewilliam<lovewilliam@gmail.com>\n"
	    "Visit http://www.bigstarstar.tk\n",progname);
}

void term()
{
    system("kill -9 `cat /tmp/h3cc.pid`");
    system("echo ''>/tmp/h3cc.pid");
};

void listDev()
{
    cout<<"-----Available Nic---"<<endl;
    h3c8021x dot1x;
    pcap_if_t *p = dot1x.getAllNic();
    while(p!=NULL)
    {
	if(strcmp(p->name,"lo")!=0&&
	    strcmp(p->name,"any")!=0)
	{
	    cout<<p->name<<endl;
	}
	p = p->next;
    }
};

void checkDevExists(char* nic)
{
    h3c8021x dot1x;
    pcap_if_t *p = dot1x.getAllNic();
    while(p!=NULL)
    {
	if(strcmp(p->name,nic)==0)
	{
	    cout<<"Found Nic:"<<nic<<endl;
	    return;
	}
	p = p->next;
    }
    cout<<"Can not found specified nic"<<endl;
    exit(-1);
}

void runAsDomain()
{
    int i, numfiles;
    pid_t pid;
    cout << "Running as daemon..." << endl;
    pid = fork();
    if (pid > 0)
	exit(0);
    else if (pid < 0)
	exit(1);
    setsid();
    pid = fork();
    if (pid > 0)
	exit(0);
    else if (pid < 0)
	exit(1);
    numfiles = getdtablesize();
    for (i = 0; i < numfiles; i++)
	close(i);
    umask(0);
    chdir("/tmp");
    char tcmd[128]={0};
    sprintf(tcmd,"echo '%d'>/tmp/h3cc.pid",getpid());
    system(tcmd);
}
