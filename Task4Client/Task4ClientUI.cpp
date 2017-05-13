#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <process.h>
#include <winsock.h>
#include <string>

using namespace std;

#define DEFAULT_PORT 5001
const int MESSAGE_MAX_SIZE = 100;
const string SEPARATOR = ": ";

char buff[MESSAGE_MAX_SIZE];
string name;
SOCKET client_socket;

bool ConnectToHost(u_short port, char *address) {
	WSADATA wsadata;

	int error = WSAStartup(0x0202, &wsadata);
	if (error) 
	{
		cerr << "Error!" << endl;
		return -1;
	}

	if (wsadata.wVersion != 0x0202) {
		cerr << "WSAStartup failed with error " << error << endl;
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN target;

	target.sin_family = AF_INET;
	target.sin_port = htons(port);
	target.sin_addr.s_addr = inet_addr(address);

	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (client_socket == INVALID_SOCKET) 
	{
		cerr << "Client failed with error " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	return !(connect(client_socket, (SOCKADDR*)&target, sizeof(target)) == SOCKET_ERROR);
}

void CloseConnection() {
	if (client_socket)
	{
		closesocket(client_socket);
	}
	WSACleanup();
}

void ReceiveAllMessages() 
{
	do
	{
		if (recv(client_socket, buff, sizeof(buff), 0) == SOCKET_ERROR) 
		{
			continue;
		}

		if (strlen(buff) == 0)
		{
			continue;
		}

		cout << " Recieved message: " << endl << "  " << buff << endl;
	} while (true);
}

int main() 
{

	if (!ConnectToHost(DEFAULT_PORT, "127.0.0.1")) {
		cerr << "Error! Problems with connection... :(" << endl;
		CloseConnection();
		return 0;
	}

	cout << "Enter your name: ";
	cin >> buff;
	name = string(buff);
	cout << endl;

	send(client_socket, buff, sizeof(buff), 0);
	cout << "Enter message: ";
	HANDLE receiveThread = (HANDLE)_beginthreadex(NULL, 0, 
		(unsigned int(__stdcall *)(void *)) ReceiveAllMessages, NULL, 0, NULL);

	string temp;
	const char *messages;
	do 
	{
		getline(cin, temp);
		if (temp.empty())
		{
			continue;
		}

		temp = name + SEPARATOR + temp;
		messages = temp.c_str();
		cout << "Enter message: ";
		send(client_socket, messages, sizeof(messages)* strlen(messages), 0);
	} while (true);

	CloseConnection();
	return 0;
}
