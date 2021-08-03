#include <iostream>
#include <WS2tcpip.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32")

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	int count = 0;

	while (true)
	{
		std::cout << "물에 푹 담군 공책 내용 새 공책으로 배끼는 중";

		for (int i = 0; i < count % 5; ++i)
		{
			std::cout << ".";
		}

		std::cout << '\n';

		std::cout << count << " 초 경과\n";
		std::cout << count / 60.0 << " 분 경과\n";

		++count;
		Sleep(1000);
		system("cls");

	}

	return(0);
}

int main()
{
	HANDLE hThread;

	hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
	



	if(WaitForSingleObject(hThread, INFINITE) != WAIT_OBJECT_0)
	{
		std::cerr << "An error occurred?." << std::endl;
	}


	return(0);
}