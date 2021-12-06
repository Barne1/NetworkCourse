#include "Engine.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>


#if CLIENT

SOCKET serverSocket = INVALID_SOCKET;

static DWORD recvWorker(void*)
{
	while (true)
	{
		static char buffer[1024];
		int recvSize = recv(serverSocket, buffer, 1024, 0);

		if (recvSize == SOCKET_ERROR || recvSize == 0)
		{
			return -1;
		}

		engPrint("%.*s", recvSize, buffer);
	}
}

int WinMain(HINSTANCE, HINSTANCE, char*, int)
{
	engInit();
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	uint8_t ipBytes[] = {46, 59, 27, 66};
	sockaddr_in connectionAddress;
	connectionAddress.sin_family = AF_INET;
	connectionAddress.sin_addr.s_addr = *(uint32_t*)ipBytes;
	connectionAddress.sin_port = htons(123);

	connect(serverSocket, (sockaddr*)&connectionAddress, sizeof(connectionAddress));
	const char* msg = "Hello, World!";

	CreateThread(
		nullptr,
		0,
		recvWorker,
		nullptr,
		0,
		nullptr);

	while (engBeginFrame())
	{
		if (engKeyPressed(Key::Space))
		{
			send(serverSocket, msg, strlen(msg), 0);
		}
		engSetColor(0x222244FF);
		engClear();
	}

	return 0;
}

#endif