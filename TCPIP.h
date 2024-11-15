﻿//#pragma once

// For TCPIP
#define _WINSOCK_DEPRECATED_NO_WARNINGS  // 系統要求
#pragma comment(lib,"ws2_32.lib")
#include <winsock2.h>
//#include "CAudio.h"

class TCPIP//: public CAudio
{
public:
	//SOCKET   SSock, CSock;
	//sockaddr SAddr, CAddr;

	// ==== 同步模式 TCPIP ====
	// ==== Sync_TCPIP ====
	int  Start_UDP_Server(SOCKET* lpSSock1, int Port);
	int  Start_UDP_Client(SOCKET* lpCSock1, sockaddr* lpCAddr1, char* IP, int SPort, int DPort);
	int  Start_TCP_Server(SOCKET* psockfd, WORD Port);
	int  Start_TCP_Client(SOCKET* psockfd, char* DIP, WORD SPort, WORD DPort);

	// ==== 非同步模式 TCPIP====
	// ==== Async_TCPIP ====
	char API_ErrMsg[256];
	void WSA_Get_ErrTxt(char* S1) {  }
	void Log(char* S1) {  }

	int  Start_TCP_Server(SOCKET* psockfd, WORD Port, DWORD EVENT, HWND Hwnd);
	int  Start_TCP_Client(SOCKET* psockfd, WORD R_Port, char* IP, DWORD EVENT, HWND Hwnd);
	int  Start_UDP_Server(SOCKET* psockfd, WORD Port, int EVENT, HWND Hwnd);
	int  Start_UDP_Client(SOCKET* psockfd, sockaddr_in* udpclient, WORD R_Port, char* IP, int EVENT, HWND Hwnd);

	//int  Get_Local_IP(char *IP);
};

int TCPIP::Start_UDP_Server(SOCKET* lpSSock1, int Port)
{
	// 1.變數宣告
	WSADATA     Wsa;      // Winsock 參數
	sockaddr_in Addr; // IP+Port+Protocol

	// 2.設定Winsock/sock
	WSAStartup(0x202, &Wsa); // 啟動Winsock
	*lpSSock1 = socket(AF_INET, SOCK_DGRAM, 0); // 開啟UDP通道
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(Port);
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 3.啟動UDP Server
	bind(*lpSSock1, (sockaddr*)&Addr, sizeof(sockaddr));
	return 0;
}

int TCPIP::Start_UDP_Client(SOCKET* lpCSock1, sockaddr* lpCAddr1, char* IP, int SPort, int DPort)
{
	// 1.變數宣告
	int         i;
	char        IP11[100];
	WSADATA     Wsa;
	LPHOSTENT   HostIP;
	sockaddr_in* Addr = (sockaddr_in*)lpCAddr1; // IP+Port+Protocol

	// 2.Check IP or Domain Name, 
	for (i = 0; i < (int)strlen(IP); i++) if (((IP[i] < '0') || (IP[i] > '9')) && (IP[i] != '.')) break;
	if (i < (int)strlen(IP))
	{
		if ((HostIP = gethostbyname(IP)) == NULL) return -1;
		else strcpy_s(IP11, sizeof(IP11), inet_ntoa(*(LPIN_ADDR)(HostIP->h_addr)));
	}
	else strcpy_s(IP11, sizeof(IP11), IP);

	// 3.Bind 指定之 Local SPort
	if (SPort > 0) Start_UDP_Server(lpCSock1, SPort);
	else
	{   // 系統自動產生 Local Port
		if ((i = WSAStartup(0x202, &Wsa)) != 0) return -2;
		if ((*lpCSock1 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) return -3;
	}

	// 3.設定Winsock/sock
	Addr->sin_family = AF_INET;
	Addr->sin_port = htons(DPort);
	Addr->sin_addr.s_addr = inet_addr(IP11);

	return 0;
}

// =======================================================================
// ==================  啟動 TCP Server(使用thread接受連線與接收資料)   ===
// =======================================================================
int  TCPIP::Start_TCP_Server(SOCKET* psockfd, WORD Port)
{
	WSADATA  wsadata;
	int      err;
	struct   sockaddr_in  tcpserver;

	// 1. 開啟 TCP Server
	if ((err = WSAStartup(0x202, &wsadata)) != 0) return -1;
	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -2;
	tcpserver.sin_family = AF_INET;
	tcpserver.sin_port = htons(Port);
	tcpserver.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*psockfd, (struct sockaddr*)&tcpserver, sizeof(tcpserver)) < 0) return -3;
	if ((err = listen(*psockfd, SOMAXCONN)) < 0) return -4;

	return 0;
}

// =======================================================================
// ==================  啟動 TCP Client   =================================
// =======================================================================
int TCPIP::Start_TCP_Client(SOCKET* psockfd, char* DIP, WORD SPort, WORD DPort)
{
	// 1.Local Variable
	int         i;
	WSADATA     wsadata;
	char        IP11[100];
	LPHOSTENT   HostIP;
	sockaddr_in tcpclient;

	// 2.Check IP or Domain Name, 
	for (i = 0; i < (int)strlen(DIP); i++) if (((DIP[i] < '0') || (DIP[i] > '9')) && (DIP[i] != '.')) break;
	if (i < (int)strlen(DIP))
	{
		if ((HostIP = gethostbyname(DIP)) == NULL) return -1;
		else strcpy_s(IP11, sizeof(IP11), inet_ntoa(*(LPIN_ADDR)(HostIP->h_addr)));
	}
	else strcpy_s(IP11, sizeof(IP11), DIP);

	// 3.Bind 指定之 Local SPort
	if (SPort > 0)
		Start_TCP_Server(psockfd, SPort);
	else
	{   // 系統自動產生 Local Port
		if ((i = WSAStartup(0x202, &wsadata)) != 0) return -2;
		if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -3;
	}

	// 4.連線遠端 DIP:DPort
	tcpclient.sin_family = AF_INET;
	tcpclient.sin_port = htons(DPort);
	tcpclient.sin_addr.s_addr = inet_addr(IP11);
	if (i = connect(*psockfd, (sockaddr*)&tcpclient, sizeof(tcpclient)) < 0) return -4;

	return 0;
}


// ===========
//#define _WINSOCK_DEPRECATED_NO_WARNINGS 1  //³o­Ó©w¸q¦bpch.hªº²Ä¤@¦æ

int  TCPIP::Start_TCP_Server(SOCKET* psockfd, WORD Port, DWORD EVENT, HWND Hwnd)
{
	WSADATA  wsadata;
	int      err;
	struct   sockaddr_in  tcpserver;

	// ±Ò°ÊWinsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// #############################
	// Set Asynchronized Mode
	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_ACCEPT | FD_WRITE | FD_READ | FD_CLOSE)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Socket Naming
	tcpserver.sin_family = AF_INET;
	tcpserver.sin_port = htons(Port);
	tcpserver.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*psockfd, (struct sockaddr*)&tcpserver, sizeof(tcpserver)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Wait for Client Login
	if ((err = listen(*psockfd, SOMAXCONN)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	return 0;
}

int TCPIP::Start_TCP_Client(SOCKET* psockfd, WORD R_Port, char* IP, DWORD EVENT, HWND Hwnd)
{
	WSADATA  wsadata;
	int      err, i, on = 1;
	struct   sockaddr_in  tcpclient, Addr;
	LPHOSTENT HostIP;
	char      IP11[100];
	int IP1, IP2, IP3, IP4;

	// ±Ò°ÊWinsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Set Asynchronized Mode
	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_CONNECT | FD_WRITE | FD_READ | FD_CLOSE)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Socket Naming
	tcpclient.sin_family = AF_INET;
	tcpclient.sin_port = htons(R_Port);
	sscanf_s(IP, "%d.%d.%d.%d", &IP1, &IP2, &IP3, &IP4);
	tcpclient.sin_addr.s_addr = IP1 + (IP2 << 8) + (IP3 << 16) + (IP4 << 24);// inet_addr(IP);

	// Connect to Server
	if (err = connect(*psockfd, (struct sockaddr*)&tcpclient, sizeof(tcpclient)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg);  return -1;
	}

	return 0;
}



int TCPIP::Start_UDP_Server(SOCKET* psockfd, WORD Port, int EVENT, HWND Hwnd)
{
	WSADATA  wsadata;
	int      err;
	struct   sockaddr_in  udpserver;

	// ±Ò°ÊWinsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -2;
	}

	// Set Asynchronized Mode
	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_READ)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -3;
	}

	// Socket Naming
	udpserver.sin_family = AF_INET;
	udpserver.sin_port = htons(Port);
	udpserver.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(*psockfd, (struct sockaddr*)&udpserver, sizeof(udpserver)) < 0)
	{
		err = WSAGetLastError();
		WSA_Get_ErrTxt(API_ErrMsg); return -4;
	}

	return 0;
}
int TCPIP::Start_UDP_Client(SOCKET* psockfd, sockaddr_in* udpclient, WORD R_Port, char* IP, int EVENT, HWND Hwnd)
{
	WSADATA     wsadata;
	int         err, i;
	LPHOSTENT   HostIP;
	char      IP11[100];
	int IP1, IP2, IP3, IP4;

	// ±Ò°ÊWinsock
	if ((err = WSAStartup(0x202, &wsadata)) != 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// TCP socket open
	if ((*psockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}

	// Set Asynchronized Mode

	if ((err = WSAAsyncSelect(*psockfd, Hwnd, EVENT, FD_READ)) < 0)
	{
		WSA_Get_ErrTxt(API_ErrMsg); return -1;
	}


	// Socket Naming
	udpclient->sin_family = AF_INET;
	udpclient->sin_port = htons(R_Port);
	//udpclient->sin_addr.s_addr=inet_addr(IP11);
	sscanf_s(IP, "%d.%d.%d.%d", &IP1, &IP2, &IP3, &IP4);
	udpclient->sin_addr.s_addr = IP1 + (IP2 << 8) + (IP3 << 16) + (IP4 << 24);// inet_addr(IP);

	return 0;
}

/*
#define  DNS_NUM  14
int  TCPIP::Get_Local_IP(char *IP)
{
	SOCKET      TSock;
	sockaddr_in TAddr;
	WSADATA     Wsa;
	HOSTENT     *HostIP;
	int         i, j, k, Len = sizeof(sockaddr);
	char        S1[2000];
	char        RIP[DNS_NUM][100] =
	{
		// US:
		"www.google.com",    "142.251.42.228",
		"8.8.8.8",                              // Google DNS
		"www.microsoft.com", "173.222.182.115",
		// TW:
		"www.nycu.edu.tw",   "203.66.34.37",
		"www.ntu.edu.tw",    "140.112.8.116",
		// CN:
		"202.103.24.68",                        // China's DNS
		"www.pku.edu.cn",    "162.105.131.160",
		"www.10086.cn",      "111.7.203.227"
	};

	IP[0] = 0;
	if ((i = WSAStartup(0x202, &Wsa)) != 0) { Log("Get_Local_IP: WSAStartup"); return -2; }

	for (k = 0; k < DNS_NUM; k++)
	{
		i = Start_TCP_Client(&TSock, (char *)&RIP[k][0], 0, 443);
		j = getsockname(TSock, (sockaddr *)&TAddr, &Len);
		if ((i == 0) && (j == 0)) break;
		else closesocket(TSock);
	}

	if (k < DNS_NUM)
	{
		inet_ntop(AF_INET, &TAddr.sin_addr, IP, 100);
		closesocket(TSock);
	}
	else
	{
		// ==== Get Local IP Address for Dynamic Checking ====
		i = gethostname(S1, sizeof(S1) - 1);
		HostIP = gethostbyname(S1);
		if (HostIP != NULL) { strcpy_s(IP, 100, inet_ntoa(*(LPIN_ADDR)(HostIP->h_addr))); return 0; }
		else
		{
			strcpy_s(IP, 100, ""); Log("Get_Local_IP: gethostbyname"); return -1;
		}
	}
	//WSACleanup();
	return 0;
}

*/
// ##########################################
// #### User Define Function & Variable #####
#define CMD_NUM    30          // 指令參數數量
#define CMD_LEN  1000          // 指令參數長度
char    Cmd[CMD_NUM][CMD_LEN]; // 儲存指令參數

int Cmd_Proc(char* S1, char* Command, char Flag)
{
	int  i, j, k1, k2;
	unsigned char* US1;
	int Command_Num = CMD_NUM, Command_Len = CMD_LEN;

	US1 = (unsigned char*)S1;
	// ==== Resoluted Command Line ====
	i = 0; k1 = 0; k2 = 0; j = strlen(S1);
	while ((i < j) && (k1 < Command_Num - 1) && (k2 < Command_Len - 1))
	{
		if (US1[i] == (unsigned char)Flag) { Command[k1 * Command_Len + k2] = 0; k1++; k2 = 0; i++; }
		else
		{
			if (US1[i] >= 0x80) { Command[k1 * Command_Len + k2] = US1[i]; i++; k2++; }
			Command[k1 * Command_Len + k2] = US1[i]; i++; k2++;
		}
	}
	Command[k1 * Command_Len + k2] = 0; for (i = k1 + 1; i < Command_Num; i++) Command[i * Command_Len + 0] = 0;
	if ((k1 == 0) || (k2 == (Command_Len - 1))) return -1; else return k1;
}