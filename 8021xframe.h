#ifndef _8021XFRAME_
#define _8021XFRAME_

//	EAP_HEAD  Packet Type
#define		EAP			0x00
#define		EAP_REQUEST		0x01
#define		EAP_RESPONSE		0x02
#define		EAP_SUCCESS		0x03
#define		EAP_FAILURE		0x04
#define		EAP_OTHER		0x0a

#define		EAPOL_START		0x01

//	EAP DATA	CODE
#define		EAP_IDENTIFY		0x01
#define		EAP_NOTIFICATION	0x02
#define		EAP_Nak			0x03
#define		EAP_MD5			0X04
#define 	EAP_GTOKENCARD		0x04//General Token Card
#define		EAP_OTP			0x05//On Time Password
#define		EAP_LOGOUT		0X08
#define		EAP_ERROR		0X09
#define		EAP_KEEPONLINE		0X14

typedef	unsigned int		DWORD;
typedef	unsigned short		WORD;
typedef	unsigned char		BYTE;

typedef	unsigned char 		u_char8;
typedef	unsigned char		u_char;
typedef	unsigned short		u_short16;
typedef	unsigned short		u_short;

#include <string>

#pragma pack(push) 
#pragma pack(1)

typedef struct PacketBase
{
    u_char8	DestMAC[6];//0x00-0x05
    u_char8	SourMAC[6];//0x06-0x0b
    
    //Remain the same//////////
    u_char8	EthType[2];//0x0c-0x0d
    u_char8	Version;//0x0e
    //////////////////////////
    u_char8	PacketType;//0x0f
    //////////////////////////
    
    u_short	Len1;//0x10-0x11
    u_char8	Code;//0x12
    u_char8	Id;//0x13
    u_short	Len2;//0x14-0x15
    u_char8	EapType;//0x16
}PacketBase;
//Out Packet
typedef struct VersionActPacket
{
    PacketBase base;
    u_char8 version[50];
}VersionActPacket;

typedef struct UsernamePacket
{
    PacketBase	base;
    u_char8 Unknown[2];
    u_char8 IP[4];
    u_char8 Username[50];
}UsernamePacket;

typedef struct PasswordPacket
{
    PacketBase	base;
    u_char8 EALen;
    u_char8 MD5Password[16];
    u_char8 Username[50];
}PasswordPacket;

typedef struct KeeponlinePacket
{
    PacketBase	base;
    u_char8 UseProxy;
    u_char8 Unknown1[2]; //0x16 0x20
    u_char8 Magic[32];
    u_char8 Unknown2[2]; //0x15 0x04
    u_char8 Ip[4];
    u_char8 Username[20];
}KeeponlinePacket;

typedef struct TokenPacket
{
    PacketBase	base;
    u_char Identifier[4]; //0x23 0x44 0x23 0x31 Token Identifier
    u_char Token[33];
}TokenPacket;

//In Packet

typedef struct MD5Challenge
{
    PacketBase	base;
    u_char len;
    u_char chap[16];
}MD5Challenge;

#pragma pack(pop) /* restore pack  config */

#endif //_8021XFRAME_
