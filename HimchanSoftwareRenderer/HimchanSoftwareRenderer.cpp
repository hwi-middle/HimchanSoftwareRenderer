#include "pch.h"

#ifdef _DEBUG
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#endif

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("HimchanRenderer");
uint16 Width = 640;
uint16 Height = 480;

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
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	RECT windowRect = { 0, 0, Width, Height };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	const uint16 ActualWindowWidth = windowRect.right - windowRect.left;
	const uint16 ActualWindowHeight = windowRect.bottom - windowRect.top;

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, ActualWindowWidth, ActualWindowHeight,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	Application appliction(Width, Height, new WinRenderer());

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
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
	{

		return 0;
	}

	//case WM_PAINT:
	//{
	//	hdc = BeginPaint(hWnd, &ps);
	//	// ----- Draw below -----

	//	DrawHelper::DrawLine(hdc, Vector2(Width, Height), Vector2(0, 0), Vector2(0, 800), RGB(255, 0, 0));
	//	DrawHelper::DrawLine(hdc, Vector2(Width, Height), Vector2(80, 80), Vector2(180, 150), RGB(0, 255, 0));
	//	DrawHelper::DrawLine(hdc, Vector2(Width, Height), Vector2(-450, 950), Vector2(70, -50), RGB(0, 0, 255));

	//	// ----- Draw above -----
	//	EndPaint(hWnd, &ps);
	//	break;
	//}
	case WM_CHAR:
	{
		switch (wParam)
		{
		case 'r':
			RECT rect;
			if (GetWindowRect(hWnd, &rect))
			{
				int width = rect.right - rect.left;
				int height = rect.bottom - rect.top;
				std::cout << "Width: " << width << std::endl;
				std::cout << "Height: " << height << std::endl;
			}
			break;
		}
		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}