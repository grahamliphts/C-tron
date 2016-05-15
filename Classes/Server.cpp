#include <ws2tcpip.h>
#include <wspiapi.h> 

#include "Server.h"
#include <iostream>
#include "cocos2d.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

Server::Server() : result(NULL)
{
	WSADATA wsaData;
	int iResult;

	struct addrinfo hints;

	//Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
		CCLOG("WSAStartup failed with error: %d\n", iResult);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
		CCLOG("getaddrinfo failed: %d\n", iResult);
}

Server::~Server()
{
	WSACleanup();
}


int Server::createSocket()
{
	int iResult;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	CCLOG("Create socket server");
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) 
	{
		CCLOG("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		CCLOG("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) 
	{
		CCLOG("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) 
	{
		CCLOG("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	do 
	{
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		CCLOG("Receive %s", recvbuf);
		if (iResult > 0) 
		{
			CCLOG("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);

			if (iSendResult == SOCKET_ERROR) 
			{
				CCLOG("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			CCLOG("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			CCLOG("Connection closing...\n");
		else 
		{
			CCLOG("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);


	return 0;
}