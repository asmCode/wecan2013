#pragma once

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#include "resource.h"

extern BOOL CALLBACK StartDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class StartDialog
{
	friend BOOL CALLBACK StartDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	HWND handle;
	HWND parent;

	int result;

	int width;
	int height;
	int bpp;
	int freq;

	void UpdateForm(bool fromGui);

	HBITMAP bmpSetup;

	void InitDialog();
	void StartPressed();
	void QuitPressed();
	void Paint(HDC hdc);

public:
	StartDialog();
	~StartDialog(void);

	void Show(HWND parent);
	int GetResult();
	void GetSelectedGraphicsMode(int &width, int &height, int &bpp, int &freq);
};
