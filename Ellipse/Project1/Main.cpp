
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "COOverlappedWindow.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	COverlappedWindow window = COverlappedWindow(hInstance);
		
	window.RegisterTheClass();
	window.Create();
	window.Show(SW_SHOW);

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}