#include "pch.h"

#ifdef _DEBUG
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#endif

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
LPCTSTR g_lpszClass = TEXT("Himchan Software Renderer");
std::function<void(uint32 Width, uint32 Height)> g_OnResize;
std::function<void(void)> g_Tick;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	uint32 InitWidth = 640;
	uint32 InitHeight = 480;
	RECT windowRect = { 0, 0, InitWidth, InitHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	const uint32 ActualWindowWidth = windowRect.right - windowRect.left;
	const uint32 ActualWindowHeight = windowRect.bottom - windowRect.top;

	hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, ActualWindowWidth, ActualWindowHeight,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	Application appliction(InitWidth, InitHeight, new WinRenderer());
	g_OnResize = std::bind(&Application::Resize, &appliction, std::placeholders::_1, std::placeholders::_2);
	g_Tick = std::bind(&Application::Tick, &appliction);

	while (true)
	{
		if (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		appliction.Tick();
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_SIZE:
	{
		uint32 Width = LOWORD(lParam);
		uint32 Height = HIWORD(lParam);
		if (g_OnResize)
		{
			g_OnResize(Width, Height);
		}

		if (g_Tick)
		{
			//g_Tick();
		}
		return 0;
	}

	case WM_ENTERSIZEMOVE:
		SetTimer(hWnd, 1, USER_TIMER_MINIMUM, NULL);
		return 0;

	case WM_EXITSIZEMOVE:
		KillTimer(hWnd, 1);
		return 0;

	case WM_TIMER:
		std::cout << "WM_TIMER" << std::endl;
		g_Tick();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}