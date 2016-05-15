#include <ws2tcpip.h>
#include <wspiapi.h> 

#include "Client.h"
#include <iostream>
#include "cocos2d.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define IP_ADDRESS "127.0.0.1"

Client::Client()
{
	WSADATA wsaData;
	ConnectSocket = INVALID_SOCKET;
	struct addrinfo hints;
	int iResult;

	//Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
		CCLOG("WSAStartup failed with error: %d\n", iResult);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(IP_ADDRESS, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) 
	{
		CCLOG("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
	}

}

Client::~Client()
{
	WSACleanup();
}

int Client::createSocket()
{
	struct addrinfo *ptr;
	int iResult = -1;

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) 
		{
			CCLOG("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) 
	{
		CCLOG("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

}

int Client::sendToServer(char *text)
{
	char *sendbuf = text;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult = -1;

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR)
	{
		CCLOG("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	CCLOG("Bytes Sent: %ld\n", iResult);
}