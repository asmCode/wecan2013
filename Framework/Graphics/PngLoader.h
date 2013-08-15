#pragma once

#include <windows.h>

class PngLoader
{
public: 
	static void LoadImage(
		const char *filename,
		int &width,
		int &height,
		int &bpp,
		BYTE *&data);

	static void ReleaseData(BYTE *&data);
};
