#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>
#include <Windows.h>

bool threadIsFinished = false;

DWORD clientLoop(void* ptr)
{
	SOCKET client = reinterpret_cast<SOCKET>(ptr);
	while (true)
	{
		char buffer[1024];
		int recvSize = recv(client, buffer, 1024, 0);
		if (recvSize == -1)
		{
			printf("Client dc:ed\n");
			return 0;
		}
		printf("%.*s\n", recvSize, buffer);
	}
	return 0;
}

void serverMain()
{
	printf("Port: ");

	unsigned short port;
	scanf_s("%hu", &port);

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in bindAdress;
	bindAdress.sin_family = AF_INET;
	bindAdress.sin_addr.s_addr = INADDR_ANY;
	bindAdress.sin_port = htons(port);

	int bindResult = bind(listenSocket, (sockaddr*)&bindAdress, sizeof(bindAdress));
	if (bindResult)
	{
		printf("Binding server listen socket failed. Error: %d\n", WSAGetLastError());
		return;
	}
	printf("Server started!\n");

	listen(listenSocket, 5);
	while (true)
	{
		SOCKET client = accept(listenSocket, NULL, NULL);
		CreateThread(
			nullptr,
			0,
			clientLoop,
			(void*)client,
			0,
			nullptr
		);
	}
}

void clientMain()
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("Could not create socket! socket() failed \n");
		return;
	}

	unsigned char ipBytes[] = { 127, 0, 0, 1 };
	unsigned int ip = *(unsigned int*)ipBytes;

	printf("Port: ");

	unsigned short port;
	scanf_s("%hu", &port);

	sockaddr_in connAdress;
	connAdress.sin_family = AF_INET;
	connAdress.sin_addr.s_addr = ip;
	connAdress.sin_port = htons(port);


	int connectionResult = connect(sock, (sockaddr*)&connAdress, sizeof(connAdress));
	if (connectionResult)
	{
		printf("Initial connection failed. Error: %d\n", WSAGetLastError());
		return;
	}

	while (true)
	{
		char sendBuffer[1024];
		gets_s(sendBuffer, 1024);

		send(sock, sendBuffer, strlen(sendBuffer), 0);
	}
}

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char selector;
	do
	{
		printf("\'s\' for server, \'c\' for client, 'e' for exit\n");
		scanf_s("%c", &selector, 1);
	} while (selector != 's' && selector != 'c' && selector != 'e');
	
	if (selector == 's')
	{
		serverMain();
	}
	else if (selector == 'c')
	{
		clientMain();
	}

	_getch();

	return 0;
}