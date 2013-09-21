#include <windows.h>

#include "StartDialog.h"
#include "DemoController.h"
#include "DemoMode.h"

#include <IO/Path.h>

#include <Windows.h>
#include <assert.h>
#include <fstream>
#include <string>
#include "GraphicsLibrary\TimeControl.h"
//#include <unzip.h>

std::string baseDataPath;

//#define TIME_SHIFT 200.0 * 1000.0f
//#define TIME_SHIFT 115.0f * 1000.0f
#define TIME_SHIFT 0

//#define START_IN_WINDOW 1
//#define SKIP_SETUP_DIALOG 1
#define DONT_USE_ARCHIVE 1

//int UnzipData(const std::string &zipFile, const std::string &dstDir)
//{
//	unzFile pizda = unzOpen(zipFile.c_str());
//
//	int res;
//
//	res = unzGoToFirstFile(pizda);
//	assert(res == 0);
//
//	bool run = true;
//	while (run)
//	{
//		unz_file_info fileInfo;
//		char fileName[MAX_PATH + 1];
//		res = unzGetCurrentFileInfo(pizda, &fileInfo, fileName, MAX_PATH, NULL, 0, NULL, 0);
//		assert(res == 0);
//
//		if (fileName[strlen(fileName) - 1] == '/' || fileName[strlen(fileName) - 1] == '\\')
//		{
//			CreateDirectory((dstDir + fileName).c_str(), NULL);
//		}
//		else
//		{
//			res = unzOpenCurrentFile(pizda);
//			assert(res == 0);
//
//			char *fileData = new char[fileInfo.uncompressed_size];
//			res = unzReadCurrentFile(pizda, fileData, fileInfo.uncompressed_size);
//			assert(res == fileInfo.uncompressed_size);
//
//			std::ofstream uncompFile((dstDir + fileName).c_str(), std::ios::binary);
//			uncompFile.write(fileData, fileInfo.uncompressed_size);
//			uncompFile.close();
//
//			delete [] fileData;
//			res = unzCloseCurrentFile(pizda);
//			assert(res == 0);
//		}
//
//		if (unzGoToNextFile(pizda) == UNZ_END_OF_LIST_OF_FILE)
//			run = false;
//	}
//
//	unzClose(pizda);
//
//	return 0;
//}

bool ShowStartDialog(int &scrWidth, int &scrHeight, int &scrBpp, int &scrFreq)
{
	StartDialog *startDialog = new StartDialog();
	startDialog ->Show(NULL);
	int result = startDialog ->GetResult();

	if (result == 1)
	{
		startDialog ->GetSelectedGraphicsMode(scrWidth, scrHeight, scrBpp, scrFreq);
		delete startDialog;
		return true;
	}
	else
	{
		delete startDialog;
		return false;
	}
}

void ReleaseDemo()
{
	demo ->Release();
	DemoController::DestroyInstance();

	/*SHFILEOPSTRUCT fileOp;
	fileOp.hwnd = 0;
	fileOp.wFunc = FO_DELETE;
	fileOp.pFrom = baseDataPath.c_str();
	fileOp.pTo = NULL;
	fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	fileOp.fAnyOperationsAborted = false;
	fileOp.hNameMappings = 0;
	fileOp.lpszProgressTitle = NULL;

	SHFileOperation(&fileOp);*/
}

void StartNormalMode(DemoMode mode)
{
	int scrWidth;
	int scrHeight;
	int scrBpp;
	int scrFreq;

#ifndef SKIP_SETUP_DIALOG
	if (!ShowStartDialog(scrWidth, scrHeight, scrBpp, scrFreq))
		return;
#else
	/*scrWidth = 1024;
	scrHeight = 768;*/
	scrWidth = 960;
	scrHeight = 540;
	scrBpp = 32;
	scrFreq = 60;
#endif

	char filename[MAX_PATH + 1];
	GetModuleFileName(GetModuleHandle(NULL), filename, MAX_PATH);
	Path moduleFilename = filename;

#ifndef DONT_USE_ARCHIVE
	char tempFolder[MAX_PATH + 1];
	GetTempPath(MAX_PATH, tempFolder);
	std::string basePath = tempFolder;
	baseDataPath = basePath + "futuris_data_fts13\\";
	UnzipData(moduleFilename.GetPath() + "demodat", basePath);
#else
	baseDataPath = moduleFilename.GetPath() + "Data\\";
#endif

	bool fullscreen = mode == DemoMode_Demo;
#ifndef START_IN_WINDOW
	fullscreen = true;
#endif

	if (!demo ->Initialize(false, mode, NULL, "Breath :: by Futuris",
		scrWidth, scrHeight, scrBpp, scrFreq, fullscreen, true))
	{
		ReleaseDemo();
		assert(false);
		return;
	}

	if (!demo ->LoadContent(baseDataPath.c_str()))
	{
		ReleaseDemo();
		assert(false);
		return;
	}

	if (!demo ->BeforeStartSetups())
	{
		ReleaseDemo();
		assert(false);
		return;
	}

	TimeControl tc;
	tc.Start();

	float time = 0;

	MSG msg;
	BOOL done = false;

	bool waiting_for_release_key = false;
	int propVal = 1;
	float timeShift = -1.0f;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				done = true;
			}
			else
			{
				float ms = tc.CheckFrameMS();
				time += ms;

#if 0

				if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
				{
					if (!waiting_for_release_key)
					{
						if (timeShift == -1)
							timeShift = time / 1000.0f;

						Log::LogT("changeGreetzProp->SetValue(%d, %.4f);", propVal, (time / 1000.0f) - timeShift);
						waiting_for_release_key = true;

						if (propVal == 1)
							propVal = 0;
						else
							propVal = 1;
					}
				}
				else
				{
					waiting_for_release_key = false;
				}
#else
//#endif

				if (!demo ->Update(time + TIME_SHIFT, ms))
					done = true;

				if (!demo ->Draw(time + TIME_SHIFT, ms))
					done = true;
#endif
			}
		}
	}

	ReleaseDemo();
}
