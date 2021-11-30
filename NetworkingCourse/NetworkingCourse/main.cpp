#include <stdio.h>
#include <WinSock2.h>

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("Could not create socket! socket() failed \n");
		return 1;
	}

	unsigned char ipBytes[] = { 127, 0, 0, 1 };
	unsigned int ip = *(unsigned int*)ipBytes;

	sockaddr_in connAdress;
	connAdress.sin_family = AF_INET;
	connAdress.sin_addr.s_addr = ip;
	connAdress.sin_port = htons(35000);


	int connectionResult = connect(sock, (sockaddr*)&connAdress, sizeof(connAdress));
	if (connectionResult)
	{
		printf("Initial connection failed. Error: %d\n", WSAGetLastError());
		return 1;
	}

	return 0;
}