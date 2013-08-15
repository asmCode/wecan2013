#include <Windows.h>

#include "../Path.h"

bool Path::IsFileExists(const char *fileName)
{
	if (GetFileAttributesA(fileName) != INVALID_FILE_ATTRIBUTES)
		return true;
	else
		return false;
}

void Path::GetAllFiles(std::vector<std::string> &files,
					   const std::string &path,
					   const std::string &filter)
{
	WIN32_FIND_DATAA fd;

	HANDLE hfile = FindFirstFileA((path + filter).c_str(), &fd);
	if (hfile != INVALID_HANDLE_VALUE)
	{
		BOOL doSearch = true;
		while (doSearch)
		{
			files.push_back(fd.cFileName);
			doSearch = FindNextFileA(hfile, &fd);
		}

		FindClose(hfile);
	}
}

