#include "StartDialog.h"

#include <Graphics/ImageLoader.h>

HBITMAP LoadGraph(HWND hwnd)
{
	uint32_t width;
	uint32_t height;
	uint32_t bpp;
	uint8_t *data;

	ImageLoader::LoadFromFile("setup.jpg", data, width, height, bpp);

	for (uint32_t i = 0; i < height * width; i++)
	{
		uint8_t tmp = const_cast<uint8_t*>(data)[i * 3 + 0];
		const_cast<uint8_t*>(data)[i * 3 + 0] = const_cast<uint8_t*>(data)[i * 3 + 2];
		const_cast<uint8_t*>(data)[i * 3 + 2] = tmp;
	}

	BITMAPINFOHEADER bth;
	BITMAPINFO bti;

	bth.biSize = sizeof(BITMAPINFOHEADER);
	bth.biWidth = width;
	bth.biHeight = height;
	bth.biPlanes = 1;
	bth.biBitCount = bpp * 8;
	bth.biCompression = BI_RGB;
	bth.biSizeImage = 0;
	bth.biXPelsPerMeter = 0;
	bth.biXPelsPerMeter = 0;
	bth.biClrUsed = 0;
	bth.biClrImportant = 0;

	bti.bmiHeader = bth;
	bti.bmiColors ->rgbBlue = 255;
	bti.bmiColors ->rgbGreen = 255;
	bti.bmiColors ->rgbRed = 255;
	bti.bmiColors ->rgbReserved = 0;

	HDC hdc = GetDC(hwnd);
	HBITMAP hbt = CreateDIBitmap(hdc, &bth, CBM_INIT, data, &bti, DIB_RGB_COLORS);
	ReleaseDC(hwnd, hdc);

	//PngLoader::ReleaseData(data);

	//delete [] data;

	return hbt;
}

BOOL CALLBACK StartDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static StartDialog *startDialog = NULL;

	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_INITDIALOG:
		startDialog = (StartDialog*)lparam;
		startDialog ->handle = hwnd;
		startDialog ->InitDialog();
		startDialog ->bmpSetup = LoadGraph(hwnd);
		return true;

	case WM_CLOSE:
		startDialog ->result = 0;
		EndDialog(hwnd, 0);
		return true;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		startDialog ->Paint(hdc);
		EndPaint(hwnd, &ps);
		return true;

	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case IDC_BTN_START:
			startDialog ->StartPressed();
			break;

		case IDC_BTN_QUIT:
			startDialog ->QuitPressed();
			break;
		}

		return true;
	}

	return false;
}

StartDialog::StartDialog()
{
}

StartDialog::~StartDialog(void)
{
}

void StartDialog::Paint(HDC hdc)
{
	LOGBRUSH logBrush;
	logBrush.lbColor = RGB(0, 0, 0);
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbHatch = 0;
	HBRUSH blackBrush = CreateBrushIndirect(&logBrush);

	HPEN pen = CreatePen(PS_NULL, 0, 0);

	HDC memHdc = CreateCompatibleDC(hdc);
	SelectObject(memHdc, bmpSetup);
	BitBlt(hdc, 0, 0, 400, 155, memHdc, 0, 0, SRCCOPY);
	SelectObject(hdc, blackBrush);
	SelectObject(hdc, pen);
	Rectangle(hdc, 0, 150, 400, 250);
	DeleteDC(memHdc);

	DeleteObject(blackBrush);
}

void StartDialog::Show(HWND parent)
{
	DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_STARTDIALOG), parent, StartDialogProc, (LPARAM)this);
}

void StartDialog::InitDialog()
{
	/*ImagePNG startImage;
	startImage.LoadImage("data\\setup.png");
	BYTE *pixels = new BYTE[400 * 155 * 4];
	BYTE *pixels2 = new BYTE[400 * 155 * 3];
	startImage.GetPixels(pixels2);
	for (int i = 0; i < 400 * 155; i++)
	{
		pixels = 
	}
	SetBitmapBits(
	bmpSetup = CreateBitmap(400, 155, 1, 32, pixels);
	startImage.Release();
	delete [] pixels;*/

	int disp_index = 0;
	DEVMODE dv;
	char text_dev_mode[128];
	int cur_sel;

	while (EnumDisplaySettings(NULL, disp_index++, &dv))
	{
		wsprintf(text_dev_mode, "%dx%dx%dbpp %dHz", dv.dmPelsWidth, dv.dmPelsHeight, dv.dmBitsPerPel, dv.dmDisplayFrequency);
		ComboBox_AddString(GetDlgItem(handle, IDC_CMB_RESOLUTION), text_dev_mode);
	}

	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dv);
	wsprintf(text_dev_mode, "%dx%dx%dbpp %dHz", dv.dmPelsWidth, dv.dmPelsHeight, dv.dmBitsPerPel, dv.dmDisplayFrequency);
	cur_sel = ComboBox_FindString(GetDlgItem(handle, IDC_CMB_RESOLUTION), 0, text_dev_mode);
	if (cur_sel != -1)
		ComboBox_SetCurSel(GetDlgItem(handle, IDC_CMB_RESOLUTION), cur_sel);
}

int StartDialog::GetResult()
{
	return result;
}

void StartDialog::StartPressed()
{
	UpdateForm(true);

	result = 1;
	EndDialog(handle, 1);
}

void StartDialog::QuitPressed()
{
	result = 0;
	EndDialog(handle, 0);
}

void StartDialog::GetSelectedGraphicsMode(int &width, int &height, int &bpp, int &freq)
{
	width = this ->width;
	height = this ->height;
	bpp = this ->bpp;
	freq = this ->freq;
}

void StartDialog::UpdateForm(bool fromGui)
{
	// resolution
	char text_dev_mode[128];
	int cur_sel = ComboBox_GetCurSel(GetDlgItem(handle, IDC_CMB_RESOLUTION));
	ComboBox_GetLBText(GetDlgItem(handle, IDC_CMB_RESOLUTION), cur_sel, text_dev_mode);
	sscanf(text_dev_mode, "%d%*c%d%*c%d%*s %d%*s", &width, &height, &bpp, &freq);
}
