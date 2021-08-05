#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32")

#define PORT 32000

int main()
{
	WSADATA		wsaData;
	SOCKET		sendingSocket;
	SOCKADDR_IN recvAddr;

	char	recvBuf[1024];
	int		bufLength = _countof(recvBuf);


	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = PORT;
	InetPton(AF_INET, TEXT("127.0.0.1"), &recvAddr.sin_addr.s_addr);

	std::cout << "sending" << std::endl;
	while (true)
	{
		sendto(sendingSocket, "와 센즈 아시는구나\0", 1024, 0, (SOCKADDR*)&recvAddr, sizeof(recvAddr));

	}

	closesocket(sendingSocket);

	WSACleanup();

	return(0);
}