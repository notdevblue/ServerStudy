#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32")

#define PORT 32000

int main()
{
	WSADATA		wsaData;
	SOCKET		receivingSocket;
	SOCKADDR_IN recvAddr;
	SOCKADDR_IN senderAddr;

	int		senderAddrSize = sizeof(senderAddr);
	char	recvBuf[1024];
	int		bufLength = _countof(recvBuf);


	WSAStartup(MAKEWORD(2, 2), &wsaData);

	receivingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port   = PORT;
	InetPton(AF_INET, TEXT("127.0.0.1"), &recvAddr.sin_addr.s_addr);

	bind(receivingSocket, (SOCKADDR*)&senderAddr, sizeof(senderAddr));

	while (true)
	{
		recvfrom(receivingSocket, recvBuf, bufLength, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);
		if (recvBuf != nullptr)
		{
			std::cout << recvBuf << " ";

		}
	}

	closesocket(receivingSocket);

	WSACleanup();

	return(0);
}