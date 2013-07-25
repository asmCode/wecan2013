#include "OpenglWindow.h"
#include <assert.h>
#include "IKeyboardCallback.h"

LRESULT CALLBACK OpenglWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static IKeyboardCallback *kbCallback;

	switch (uMsg)
	{
	case WM_CREATE:
		{
			CREATESTRUCT *cs = (CREATESTRUCT*)lParam;
			kbCallback = (IKeyboardCallback*)cs->lpCreateParams;
		}
		return 0;

	case WM_KEYDOWN:
		if (kbCallback != NULL)
			kbCallback->OnKeyDown(wParam);

		return 0;

	case WM_MOUSEACTIVATE:
		SetFocus(hWnd);
		return 0;

	case WM_SIZE:
		return 0;

	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}

	case WM_LBUTTONDOWN:
		if (kbCallback != NULL)
			kbCallback->OnLeftMouseDown();
		return 0;

	case WM_DESTROY:
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

OpenglWindow::OpenglWindow(void)
{
}

OpenglWindow::~OpenglWindow(void)
{
	Release();
}

bool OpenglWindow::Initialize(HWND parentWindow, const char * title, int width, int height,
				int bpp, int freq, bool fullscreen, bool createOwnWindow, void *params)
{
	if (fullscreen)
		createOwnWindow = true;

	this ->windowWidth = width;
	this ->windowHeight = height;
	this ->fullscreen = fullscreen;
	this ->createOwnWindow = createOwnWindow;
	this ->parentWindow = parentWindow;

	DWORD windowStyle = 0;
	DWORD windowExStyle = 0;

	HINSTANCE hinstance = GetModuleHandle(NULL);
	assert(hinstance != INVALID_HANDLE_VALUE);

	if (createOwnWindow)
	{
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = OpenglWindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = "OpenglWindow";

		if (!RegisterClass(&wc))
		{
			Release();
			return false;
		}

		if (fullscreen)
		{
			windowStyle = WS_POPUP;
			windowExStyle = WS_EX_TOPMOST;
		}
		else
		{
			if (parentWindow != NULL)
				windowStyle = WS_CHILD;
			else
				windowStyle = WS_POPUP;
		}

		hwnd = CreateWindowEx(windowExStyle, "OpenglWindow", title,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | windowStyle, 0, 0, width, height,
			parentWindow, NULL, hinstance, params);

		if (hwnd == 0)
		{
			Release();
			return false;
		}
	}
	else
	{
		hwnd = parentWindow;
	}

	if (fullscreen)
	{
		DEVMODE screen_settings;
		memset(&screen_settings, 0, sizeof(DEVMODE));
		screen_settings.dmSize = sizeof(screen_settings);
		screen_settings.dmPelsWidth = width;
		screen_settings.dmPelsHeight = height;
		screen_settings.dmBitsPerPel = bpp;
		screen_settings.dmDisplayFrequency = freq;
		screen_settings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

		if (ChangeDisplaySettings(&screen_settings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			Release();
			return false;
		}

		ShowCursor(false);
	}

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof (PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		bpp,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	hdc = GetDC(hwnd);
	if (hdc == 0)
	{
		Release();
		return false;
	}

	UINT pixel_format;

	pixel_format = ChoosePixelFormat(hdc, &pfd);
	if (pixel_format == 0)
	{
		Release();
		return false;
	}

	if (!SetPixelFormat(hdc, pixel_format, &pfd))
	{
		Release();
		return false;
	}

	hrc = wglCreateContext(hdc);
	if (hrc == 0)
	{
		Release();
		return false;
	}

	if (!wglMakeCurrent(hdc, hrc))
	{
		Release();
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return true;
}

void OpenglWindow::Release()
{
	if (fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	if (hrc)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);
		hrc = NULL;
	}

	if (hdc)
	{
		ReleaseDC(hwnd, hdc);
		hdc = NULL;
	}

	if (hwnd != NULL && createOwnWindow)
	{
		DestroyWindow(hwnd);
		hwnd = NULL;

		UnregisterClass("OpenglWindow", GetModuleHandle(NULL));
	}
}

void OpenglWindow::Resize(int width, int height)
{
	this ->windowWidth = width;
	this ->windowHeight = height;

	SetWindowPos(hwnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER);
}

int OpenglWindow::GetWindowWidth()
{
	return windowWidth;
}

int OpenglWindow::GetWindowHeight()
{
	return windowHeight;
}

void OpenglWindow::SetCurrentContext()
{
	assert(wglMakeCurrent(hdc, hrc));
}

void OpenglWindow::SwapBuffers()
{
	::SwapBuffers(hdc);
}

HDC OpenglWindow::GetHDC()
{
	return hdc;
}

HWND OpenglWindow::GetHwnd()
{
	return hwnd;
}

HGLRC OpenglWindow::GetGlContext()
{
	return hrc;
}

int OpenglWindow::Log2PX(float logX)
{
	return (int)((float)windowWidth * logX);
}

int OpenglWindow::Log2PY(float logY)
{
	return (int)((float)windowHeight * logY);
}

float OpenglWindow::P2LogX(int pX)
{
	return (float)pX / (float)windowWidth;
}

float OpenglWindow::P2LogY(int pY)
{
	return (float)pY / (float)windowHeight;
}
