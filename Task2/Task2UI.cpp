#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main()
{
	DWORD dwRetVal;
	IPAddr destIp;
	IPAddr srcIp = 0;
	ULONG macAddr[2];
	ULONG physAddrLen = 6;
	char *destIpString;

	BYTE *bPhysAddr;
	destIpString = "192.168.0.102";
	destIp = inet_addr(destIpString);

	memset(&macAddr, 0xff, sizeof(macAddr));

	cout << "Sending ARP request for IP address " << destIpString << endl;

	dwRetVal = SendARP(destIp, srcIp, &macAddr, &physAddrLen);
	if (dwRetVal == NO_ERROR)
	{
		bPhysAddr = (BYTE *)&macAddr;
		if (physAddrLen)
		{
			cout << "MAC-address: ";
			for (int i = 0; i < (int)physAddrLen; i++)
			{
				if (i == physAddrLen - 1)
				{
					cout << hex << (int)bPhysAddr[i] << endl;
				}
				else
				{
					cout << hex << (int)bPhysAddr[i] << "-";
				}
			}
		}
		else
		{
			cout << "Error! Returned physical address has zero length!" << endl;
		}
	}
	else
	{
		cout << "Error! SendArp command failed with error " << dwRetVal <<  ": ";

		switch (dwRetVal)
		{
		case ERROR_GEN_FAILURE:
			cout << "ERROR_GEN_FAILURE" << endl;			
			break;
		case ERROR_INVALID_PARAMETER:
			cout << "ERROR_INVALID_PARAMETER" << endl;			
			break;
		case ERROR_INVALID_USER_BUFFER:
			cout << "ERROR_INVALID_USER_BUFFER" << endl;
			break;
		case ERROR_BAD_NET_NAME:
			cout << "ERROR_BAD_NET_NAME" << endl;
			break;
		case ERROR_BUFFER_OVERFLOW:
			cout << "ERROR_BUFFER_OVERFLOW" << endl;
			break;
		case ERROR_NOT_FOUND:
			cout << "ERROR_NOT_FOUND" << endl;
			break;
		default:
			cout << "UNKNOWN ERROR" << endl;
			break;
		}
	}

	cout << endl;

	system("pause");
	return 0;
}


