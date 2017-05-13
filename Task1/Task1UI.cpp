#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include <string>
using namespace std;

int main() 
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	const int length = 255;
	char hostname[length];

	gethostname(hostname, length);
	cout << "Your PC localhost name:" << hostname << endl << endl;

	cout << "Enter host name or ip-address:" << endl;
	string str;
	cin >> str;

	HOSTENT* host;
	ULONG addr = inet_addr(str.c_str());

	if (addr != INADDR_NONE) 
	{
		host = gethostbyaddr((char*)&addr, sizeof(ULONG), AF_INET);
		cout << endl << "Get host by ip-address" << endl;
	}
	else 
	{
		host = gethostbyname(str.c_str());
		cout << endl << "Get host by name" << endl;
	}

	if (host != NULL)
	{
		cout << " Host name:" << host->h_name << endl;		
	
		int i = 0;
		while (host->h_aliases[i] != NULL) 
		{
			cout << " Alias name: " << host->h_aliases[i] << endl;
			i++;
		}

		i = 0;
		cout << " Address list:" << endl;
		while (host->h_addr_list[i] != NULL) 
		{
			cout << "  " << inet_ntoa(*(in_addr*)host->h_addr_list[i]) << endl;
			i++;
		}
	}
	else 
	{
		cerr << "Error! Can't find host " << str << endl;
	}

	WSACleanup();
	cout << endl;

	system("pause");
	return 0;
}