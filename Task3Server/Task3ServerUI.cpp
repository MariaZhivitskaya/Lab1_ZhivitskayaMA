#pragma comment(lib, "Ws2_32.lib")

#include <winsock.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main()
{
	char buffer[128];
	unsigned short port = 5001;
	int retVal;
	int fromLen;
	int socket_type = SOCK_STREAM;
	struct sockaddr_in local, from;
	WSADATA wsaData;
	SOCKET listen_socket, msgsock;

	if (WSAStartup(0x101, &wsaData) == SOCKET_ERROR)
	{
		cerr << "WSAStartup failed with error " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	listen_socket = socket(AF_INET, socket_type, 0);
	if (listen_socket == INVALID_SOCKET) 
	{
		cerr << "Server error while opening socket: " << WSAGetLastError() << endl;		
		WSACleanup();
		return -1;
	}
	
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(port);

	if (bind(listen_socket, (struct sockaddr*)&local, sizeof(local)) == SOCKET_ERROR)
	{
		cerr << "bind command failed with error: " << WSAGetLastError() << endl;		
		WSACleanup();
		return -1;
	}

	if (listen(listen_socket, 5) == SOCKET_ERROR) 
	{
		cerr << "listen command failed with error: " << WSAGetLastError() << endl;		
		WSACleanup();
		return -1;
	}

	cout << "Server listening on port " << port << ", protocol TCP" << endl;

	fromLen = sizeof(from);
	msgsock = accept(listen_socket, (struct sockaddr*)&from, &fromLen);
	if (msgsock == INVALID_SOCKET) 
	{
		cerr << "accept command failed with error: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	cout << "Accepted connection from " << inet_ntoa(from.sin_addr) << ", port" << htons(from.sin_port) << endl;

	retVal = recv(msgsock, buffer, sizeof(buffer), 0);
	if (retVal == SOCKET_ERROR)
	{
		cerr << "recv command failed with error: " << WSAGetLastError() << endl;
		closesocket(msgsock);
		return -1;
	}

	if (retVal == 0) 
	{
		cout << "Client closed connection" << endl;
		closesocket(msgsock);
		return -1;
	}

	cout << endl;
	cout << " Recieved " << retVal << " bytes from client" << endl;
	cout << " Message: " << buffer << endl << endl;

	cout << "Enter message: ";
	gets_s(buffer, 128);

	retVal = send(msgsock, buffer, 128 - 1, 0);
	if (retVal == SOCKET_ERROR)
	{
		cerr << "send command failed with error: " << WSAGetLastError() << endl;
	}

	closesocket(msgsock);
	WSACleanup();	
	cout << endl;

	system("pause");
	return 0;
}


