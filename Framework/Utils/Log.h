#pragma once

#include <string>
#include <fstream>

class Log
{
private:
	static std::ofstream fileStream;
	static std::stringstream logSStream;
	static char dstBuf[1024];
	static char timeBuff[80];
	
	static bool logToConsole;
	static bool logToFile;
	static bool logThreadName;

	static void UpdateTime();
	static void UpdateLog();
		
public:
	static const char* DefaultLogFileName;
	
	static void StartLog(bool logToConsole,
						 bool logToFile,
						 bool logThreadName,
						 const char *logFileName = DefaultLogFileName);
	
	static void LogT(const char *formatString, ...);
	static void LogW(const char *formatString, ...);
	static void LogE(const char *formatString, ...);
};
