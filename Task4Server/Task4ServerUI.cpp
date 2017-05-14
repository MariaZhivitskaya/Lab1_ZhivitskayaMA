#pragma comment(lib, "Ws2_32.lib")

#include <map>
#include <string>
#include <iostream>
#include <winsock.h>
#include <process.h>

using namespace std;

#define DEFAULT_PORT 5001

const int MESSAGE_MAX_SIZE = 100;
const string SEPARATOR = ": ";
map<string, SOCKET> nameSocket;
char buff[MESSAGE_MAX_SIZE];
SOCKET server_socket;
WSADATA wsaData;

void SendMessagesAboutNewClient(void *client_socket, string name)
{
	SOCKET socket = *((SOCKET*)client_socket);
	string tmp("\nNew client:");
	tmp.append(name);
	cout << " Sending message to all clients:" << buff << endl;

	for (map<string, SOCKET>::iterator it = nameSocket.begin(); it != nameSocket.end(); ++it) 
	{
		if (it->first != name)
		{
			send(it->second, tmp.c_str(), 20, 0);
		}
	}
}

void SendMessages(void* client_socket) 
{
	SOCKET socket = *((SOCKET*)client_socket);	
	string name;

	while (true)
	{
		if (recv(socket, buff, sizeof(buff), 0) == SOCKET_ERROR)
		{ 
			cerr << endl << "Client closed connection" << endl;
			break;
		}

		string tmp(buff);
		cout << buff << endl;
		name = tmp.substr(0, tmp.find(SEPARATOR));

		for (map<string, SOCKET>::iterator it = nameSocket.begin(); it != nameSocket.end(); ++it) 
		{
			if (it->first == name)
			{
				continue;
			}

			send(it->second, buff, sizeof(buff), 0);
		}

		Sleep(1000);
	}
}

int main() 
{
	cout << "Server is working..." << endl;

	int error = WSAStartup(0x0202, &wsaData);
	if (error)
	{
		cerr << "WSAStartup failed with error " << error << endl;
		WSACleanup();
		return -1;
	}

	if (wsaData.wVersion != 0x0202) {
		cerr << "Server failed with error!" << endl;
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == INVALID_SOCKET)
	{
		cerr << "Server failed with error " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	if (bind(server_socket, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) 
	{
		cerr << "bind command failed with error: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	listen(server_socket, 50);
	cout << "Server is listening on port " << DEFAULT_PORT << endl;

	SOCKET client_socket;

	while (true)
	{
		client_socket = accept(server_socket, NULL, NULL);
		recv(client_socket, buff, sizeof(buff), 0);
		string name(buff);
		nameSocket[name] = client_socket;
		cout << endl << "New client: " << name << endl;
		SendMessagesAboutNewClient(&client_socket, name);

		HANDLE sendThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *)) SendMessages,
			(LPVOID)&client_socket, 0, NULL);
	}

	return 0;
}
