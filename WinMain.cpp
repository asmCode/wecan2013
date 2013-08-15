#include <windows.h>
#include <AtlConv.h>

#include "MemoryLeak.h"
#include <Utils\Log.h>
#include <IO\BinaryReader.h>
#include "DemoMode.h"

#define MANUAL_START 1

void StartEditMode(const wchar_t *enliverAccessPath);
void StartNormalMode(DemoMode demoMode);

DemoMode CheckDemoMode(LPWSTR *argv, int argc)
{
#if MANUAL_START
	return DemoMode_Freelook;
#endif

	USES_CONVERSION;
	
	if (argc < 2)
		return DemoMode_Demo;

	const char *arg1 = W2A(argv[1]);

	if (strcmp(arg1, "edit") == 0)
		return DemoMode_Editor;

	if (strcmp(arg1, "free") == 0)
		return DemoMode_Freelook;

	return DemoMode_Unknow;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, int)
{
	//_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int argc = 0;
	LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	Log::StartLog(false, true, false);

	switch (CheckDemoMode(argv, argc))
	{
	case DemoMode_Demo:
		StartNormalMode(DemoMode_Demo);
		break;

	case DemoMode_Freelook:
		StartNormalMode(DemoMode_Freelook);
		break;
	}

	return 0;
}
