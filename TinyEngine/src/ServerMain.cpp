#include "Engine.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include <stdio.h>
#include "Player.h"

#if SERVER

#define USER_MAX 8
struct User
{
	bool active = false;
	SOCKET sock;
};
User users[USER_MAX];

void Broadcast(const char* msg, int msgSize)
{
	for (int i = 0; i < USER_MAX; i++)
	{
		if (!users[i].active)
			continue;

		send(users[i].sock, msg, msgSize, 0);
	}
}

static DWORD receiveWorker(void* ptr)
{
	User* user = (User*)ptr;
	char buffer[1024];

	while (true)
	{
		int recvSize = recv(user->sock, buffer, 1024, 0);
		if (recvSize == SOCKET_ERROR || recvSize == 0)
		{
			user->active = false;

			int userIdx = user - users;
			engPrint("User %d disconnected.", userIdx);
			return 0;
		}

		engPrint("%.*s", recvSize, buffer);
	}
}

DWORD acceptWorker(void* ptr)
{
	SOCKET listenSock = reinterpret_cast<SOCKET>(ptr);
	while (true)
	{
		sockaddr_in acceptAddr;
		int acceptAddrLen = sizeof(acceptAddr);
		
		SOCKET newUserSock = accept(listenSock, (sockaddr*)&acceptAddr, &acceptAddrLen);
		

		int newUserIdx = -1;
		for (int i = 0; i < USER_MAX; i++)
		{
			if (users[i].active)
				continue;
			newUserIdx = i;
			break;
		}
		if (newUserIdx == -1)
		{
			closesocket(newUserSock);
			continue;
		}
		User* newUser = &users[newUserIdx];
		newUser->active = true;
		newUser->sock = newUserSock;

		engPrint("[%d.%d.%d.%d:%d] connected.",
			acceptAddr.sin_addr.s_net,
			acceptAddr.sin_addr.s_host,
			acceptAddr.sin_addr.s_lh,
			acceptAddr.sin_addr.s_impno,
			ntohs(acceptAddr.sin_port)
		);


		CreateThread(
			nullptr,
			0,
			receiveWorker,
			newUser,
			0,
			nullptr
		);
	}
}


int WinMain(HINSTANCE, HINSTANCE, char*, int)
{
	engInit();
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET)
	{
		return 1;
	}

	sockaddr_in bindAddress;
	bindAddress.sin_family = AF_INET;
	bindAddress.sin_addr.s_addr = ADDR_ANY;
	bindAddress.sin_port = htons(123);

	if (bind(listenSock, (sockaddr*)&bindAddress, sizeof(bindAddress)))
	{
		engPrint("bind() failed: %s", WSAGetLastError());
	}
	listen(listenSock, 10);
	CreateThread(
		nullptr,
		0,
		acceptWorker,
		(void*)listenSock,
		0,
		nullptr
	);

	Player player;
	player.x = 50;
	player.y = 200;

	while (engBeginFrame())
	{
		engSetColor(0x442222FF);
		engClear();

		player.Draw();
	}

	return 0;
}

#endif