#pragma once

#include <windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <gl\glu.h>

class OpenglWindow
{
private:
	HWND hwnd;
	HWND parentWindow;

	int windowWidth;
	int windowHeight;

	HDC hdc;
	HGLRC hrc;

	bool fullscreen;
	bool createOwnWindow;

public:
	OpenglWindow(void);
	~OpenglWindow(void);

	bool Initialize(HWND parent, const char * title, int width, int height,
		int bpp, int freq, bool fullscreen, bool createOwnWindow, void *params);
	void Release();

	void Resize(int width, int height);
	int GetWindowWidth();
	int GetWindowHeight();

	void SetCurrentContext();
	void SwapBuffers();

	HDC GetHDC();
	HWND GetHwnd();
	HGLRC GetGlContext();

	int Log2PX(float logX);
	int Log2PY(float logY);

	float P2LogX(int pX);
	float P2LogY(int pY);
};
