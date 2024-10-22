#include <windows.h>
#include <WindowsX.h>
#include <MMSystem.h>
#include <ddraw.h>


#define APP_NAME "DirectX App"
#define TITLE "DirectX App"
#define LXIMAGE 640 // dimensions de la fenêtre de jeu
#define LYIMAGE 480

HWND g_hWnd;
HINSTANCE g_hInstance;
BOOL g_bActive = TRUE;

void InitHInstance(HINSTANCE hInstance)
{
	g_hInstance = hInstance;
}

static BOOL WinInit(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSA wc;
	POINT totalDim, iconDim;
	RECT rcRect;

	InitHInstance(hInstance);

	// Set up and register window class.
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc; // Default window procedure
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); // Default icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Default cursor
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APP_NAME;

	if (!RegisterClassA(&wc))
	{
		MessageBox(NULL, "Failed to register window class.", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	BOOL fullscreen = false;
	// Create a window.
	if (fullscreen)
	{
		g_hWnd = CreateWindowExA(
			WS_EX_TOPMOST,
			APP_NAME,
			TITLE,
			WS_POPUP,
			0, 0,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			NULL,
			NULL,
			hInstance,
			NULL
		);
	}
	else
	{
		int sx = GetSystemMetrics(SM_CXSCREEN);
		int sy = GetSystemMetrics(SM_CYSCREEN);
		RECT WindowRect = { (sx - LXIMAGE) / 2, (sy - LYIMAGE) / 2, (sx + LXIMAGE) / 2, (sy + LYIMAGE) / 2 };

		AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);

		g_hWnd = CreateWindowA(
			APP_NAME,
			TITLE,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			WindowRect.left,
			WindowRect.top,
			WindowRect.right - WindowRect.left,
			WindowRect.bottom - WindowRect.top,
			NULL,
			NULL,
			hInstance,
			NULL
		);
	}

	if (!g_hWnd)
	{
		MessageBox(NULL, "Failed to create window.", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);
	SetFocus(g_hWnd);

	totalDim.x = LXIMAGE;
	totalDim.y = LYIMAGE;
	iconDim.x = 0;
	iconDim.y = 0;

	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if (!WinInit(hInstance, lpCmdLine, nCmdShow))
	{
		return -1;
	}

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!g_bActive)
			{
				WaitMessage();
			}
		}
	}

	return (int)msg.wParam;
}
