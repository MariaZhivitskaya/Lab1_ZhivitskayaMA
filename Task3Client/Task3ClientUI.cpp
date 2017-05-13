#pragma comment(lib, "Ws2_32.lib")

#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

int main()
{
	char buffer[128];
	char server_name[20];
	unsigned short port = 5001;
	int retVal;
	unsigned int addr;
	int socket_type;
	struct sockaddr_in server;
	struct hostent *hp;
	WSADATA wsaData;
	SOCKET  conn_socket;
	socket_type = SOCK_STREAM;

	if (WSAStartup(0x101, &wsaData) == SOCKET_ERROR)
	{
		cerr << "WSAStartup failed with error " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	cout << "Enter server IP-address: ";
	gets_s(server_name, 20);	

	if (isalpha(server_name[0])) 
	{
		hp = gethostbyname(server_name);
	}
	else 
	{
		addr = inet_addr(server_name);
		hp = gethostbyaddr((char *)&addr, 4, AF_INET);
	}

	if (hp == NULL)
	{
		cerr << "Client error while resolving address " << server_name << ": " << WSAGetLastError() << endl;		
		WSACleanup();
		return -1;
	}
	
	conn_socket = socket(AF_INET, socket_type, 0);
	if (conn_socket == INVALID_SOCKET) 
	{
		cerr << "Client error while opening socket: " << WSAGetLastError() << endl;		
		WSACleanup();
		return -1;
	}

	memset(&server, 0, sizeof(server));
	memcpy(&(server.sin_addr), hp->h_addr, hp->h_length);
	server.sin_family = hp->h_addrtype;
	server.sin_port = htons(port);

	cout << "Client connecting to " << hp->h_name << endl << endl;
	if (connect(conn_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) 
	{
		cerr << "connect command failed with error: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	cout << "Enter message: ";
	gets_s(buffer, 128);	

	retVal = send(conn_socket, buffer, sizeof(buffer), 0);
	if (retVal == SOCKET_ERROR) 
	{
		cerr << "send command failed with error: " << WSAGetLastError() << endl;		
		WSACleanup();
		return -1;
	}

	cout << "Sent message: " << buffer << endl << endl;

	retVal = recv(conn_socket, buffer, sizeof(buffer), 0);
	if (retVal == SOCKET_ERROR) 
	{
		cerr << "recv command failed with error: " << WSAGetLastError() << endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	if (retVal == 0)
	{
		cout << "Server closed connection" << endl;
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	buffer[retVal] = 0;
	cout << endl;
	cout << " Recieved " << retVal << " bytes from server" << endl;
	cout << " Message: " << buffer << endl;

	closesocket(conn_socket);
	WSACleanup();
	cout << endl;

	system("pause");
	return 0;
}
