//#include <time.h>
#include "Game.h"
#include "GameLoopControl.h"
bool programLoop;

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		PostMessage(hwnd, WM_DESTROY, 0, 0);
		return 0;
	case WM_DESTROY:
		programLoop = false;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS wc = { 0 };
	wc.style = 0/*CS_HREDRAW | CS_VREDRAW*/;
	wc.lpfnWndProc = WinProc;
	wc.hInstance = hInstance;
	wc.hIcon = (HICON)LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = (HCURSOR)LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//Transparent = (HBRUSH)0; //(HBRUSH)GetStockObject(WHITE_BRUSH); //(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "ProjectF";

    return RegisterClass(&wc);
}


HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd = CreateWindow(
		"ProjectF",
		"Project Fairy",
		(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN)/*WS_OVERLAPPEDWINDOW*/,
		550, 220,
		800, 600,
		0,
		NULL,
		hInstance,
		0);

	if(hwnd == 0)
		return 0;

	UpdateWindow(hwnd);
	ShowWindow(hwnd, nCmdShow);

   return hwnd;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameLoopControl loopCon(35.00, 5);
	bool updateProperly = false;

	programLoop = true;

	MyRegisterClass(hInstance);
	HWND hwnd = InitInstance(hInstance, nCmdShow);

	if(hwnd == 0)
		return 0;

	Game spelet(hwnd, hInstance);

	if(!spelet.initGame())
		return 0;

	MSG msg = {0};

	loopCon.initilize_NEXT_GAME_TICK();
	while(programLoop)
	{
		
		while(loopCon.LETS_UPDATE() && !loopCon.SKIPPING_UPDATE())
		{
			updateProperly = true;
			spelet.gameInput();
			spelet.gameUpdate();
			loopCon.update_NEXT_GAME_TICK();
		}
		loopCon.resetLoops();
		if(updateProperly)
		{
			loopCon.update_INTERPOLATION();
			spelet.gameGraphics(loopCon.get_INTERPOLATION());
		}

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}