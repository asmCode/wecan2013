#include <windows.h>
#include <AtlConv.h>

#include "MemoryLeak.h"
#include <Utils\Log.h>
#include <IO\BinaryReader.h>

void StartNormalMode();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, int)
{
	//_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int argc = 0;
	LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	Log::StartLog(false, true, false);

	StartNormalMode();

	return 0;
}
