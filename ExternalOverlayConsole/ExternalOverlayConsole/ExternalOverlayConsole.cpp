// ExternalOverlayConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	HWND window = NULL;

	while (true)
	{
		window = GetForegroundWindow();
		char data[256];

		GetWindowTextA(window, data, 256);
		std::cout << "... " << data << std::endl;
		Sleep(1000);
	}
	return 0;
}

