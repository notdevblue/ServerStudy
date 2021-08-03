#include <Windows.h>
#include <iostream>


thread_local int localOne = 10;
_declspec(thread) int localTwo = 20;


DWORD WINAPI LocalStroage(LPVOID lpParam)
{
	std::cout << localOne << " " << localTwo << "\n";


	return(0);
}

int main()
{
	HANDLE hThreads[3];

	for (int i = 0; i < 3; ++i)
	{
		hThreads[i] = CreateThread(NULL, 0, LocalStroage, NULL, 0, NULL);
	}


	if (WaitForMultipleObjects(_countof(hThreads), hThreads, true, INFINITE) != WAIT_OBJECT_0)
	{
		std::cerr << "An error occurred\n";
		return(-1);
	}

	return(0);
}