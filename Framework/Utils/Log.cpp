#include "Log.h"
#include <sstream>
#include <time.h>
#include <stdarg.h>
#include <Windows.h>
#include <debugapi.h>

const char* Log::DefaultLogFileName = "log.txt";
bool Log::logToConsole;
bool Log::logToFile;
bool Log::logThreadName;

std::ofstream Log::fileStream;
std::stringstream Log::logSStream;
char Log::dstBuf[1024];
char Log::timeBuff[80];

void Log::UpdateTime()
{
	time_t	rawTime;
	tm		*timeInfo;
	
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	
	strftime(timeBuff, 70, "%X", timeInfo);
}

void Log::UpdateLog()
{
	logSStream.str("");
	logSStream << "[";
	
	if (logThreadName)
	{		
		logSStream << "-";
	}
	
	logSStream << timeBuff << "-T    ] " << dstBuf << std::endl;
}

void Log::StartLog(bool logToConsole,
				   bool logToFile,
				   bool logThreadName,
				   const char *logFileName)
{
	Log::logToConsole = logToConsole;
	Log::logToFile = logToFile;
	Log::logThreadName = logThreadName;

	if (Log::logToFile)
	{
		if (fileStream.is_open())
			fileStream.close();

		fileStream.open(logFileName, std::ios::trunc);
	}
}

void Log::LogT(const char *formatString, ...)
{	
	va_list argptr;
	va_start(argptr, formatString);
	vsprintf(dstBuf, formatString, argptr);
	va_end(argptr);
	
	UpdateTime();
	UpdateLog();
	
	if (logToConsole)
		OutputDebugStringA(logSStream.str().c_str());

	if (logToFile)
	{
		fileStream << logSStream.str();
		fileStream.flush();
	}
}

void Log::LogW(const char *formatString, ...)
{	
	va_list argptr;
	va_start(argptr, formatString);
	vsprintf(dstBuf, formatString, argptr);
	va_end(argptr);
	
	UpdateTime();
	UpdateLog();
	
	if (logToConsole)
		printf("%s", logSStream.str().c_str());
	
	if (logToFile)
	{
		fileStream << logSStream.str();
		fileStream.flush();
	}
}

void Log::LogE(const char *formatString, ...)
{
	va_list argptr;
	va_start(argptr, formatString);
	vsprintf(dstBuf, formatString, argptr);
	va_end(argptr);
	
	UpdateTime();
	UpdateLog();
	
	if (logToConsole)
		printf("%s", logSStream.str().c_str());
	
	if (logToFile)
	{
		fileStream << logSStream.str();
		fileStream.flush();
	}
}

