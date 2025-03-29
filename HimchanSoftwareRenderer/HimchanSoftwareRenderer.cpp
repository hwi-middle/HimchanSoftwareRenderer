#include "pch.h"

#ifdef _DEBUG
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#endif

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
LPCTSTR g_lpszClass = TEXT("Himchan Software Renderer");
TCHAR g_title[256];
std::function<void(uint32 Width, uint32 Height)> g_OnResize;
std::function<void(void)> g_Tick;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
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

	uint32 initWidth = 640;
	uint32 initHeight = 480;
	RECT windowRect = { 0, 0, initWidth, initHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	const uint32 actualWindowWidth = windowRect.right - windowRect.left;
	const uint32 actualWindowHeight = windowRect.bottom - windowRect.top;

	HWND hWnd = CreateWindow(g_lpszClass, g_lpszClass, WS_OVERLAPPEDWINDOW,
	                         CW_USEDEFAULT, CW_USEDEFAULT, actualWindowWidth, actualWindowHeight,
	                         NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	Application application(initWidth, initHeight, new WinRenderer(), new Input());
	g_OnResize = std::bind(&Application::Resize, &application, std::placeholders::_1, std::placeholders::_2);
	g_Tick = std::bind(&Application::Tick, &application);

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

		application.Tick();
		_stprintf_s(g_title, _T("Himchan Software Renderer (%.2f fps)"), application.GetFps());
		SetWindowText(hWnd, g_title);
	}

	return static_cast<int>(Message.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_SIZE:
	{
		const uint32 width = LOWORD(lParam);
		const uint32 height = HIWORD(lParam);
		if (g_OnResize)
		{
			g_OnResize(width, height);
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
		g_Tick();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}