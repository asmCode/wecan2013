#ifndef PATH_H
#define PATH_H

#include <Math/MathUtils.h>
#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>

class Path
{
private:
	std::string file_path;
	std::string file_name;
	std::string file_ext;

public:
	Path(std::string path)
	{
		if (path.empty())
			return;

		std::size_t strLen;
		std::size_t lastSlash = MathUtils::Max(static_cast<int>(path.find_last_of("/")),
										       static_cast<int>(path.find_last_of("\\")));

		// splitting path and filename + ext
		strLen = path.size();
		if (lastSlash != -1)
		{
			file_path = path.substr(0, lastSlash + 1);

			if (lastSlash + 1 != strLen)
				file_name = path.substr(lastSlash + 1, -1);
		}
		else
			file_name = path;

		// splitting filename and ext
		strLen = file_name.size();
		std::size_t lastDot = file_name.find_last_of(".");		
		if (lastDot != -1 && lastDot != 0)
		{
			if (lastDot + 1 != strLen)
				file_ext = file_name.substr(lastDot + 1, -1);

			file_name = file_name.substr(0, lastDot);
		}
	}

	std::string GetPath()
	{
		return file_path;
	}

	std::string GetExt()
	{
		return file_ext;
	}

	std::string GetFilename()
	{
		return file_name;
	}

	std::string GetFilenameExt()
	{
		if (!file_ext.empty())
			return file_name + "." + GetExt();
		else
			return file_name;
	}

	std::string GetFullPath()
	{
		return file_path + GetFilenameExt();
	}

	// ads slash at the end of the string, if required
	static void FixFoldername(std::string &folderName)
	{
		if (MathUtils::Max(static_cast<int>(folderName.find_last_of("/")),
			               static_cast<int>(folderName.find_last_of("\\"))) == folderName.size() - 1)
			return;

		folderName += "/";
	}

	static bool IsFileExists(const char *fileName);

	static void GetAllFiles(std::vector<std::string> &files,
							const std::string &path,
							const std::string &filter);

	static bool GetFileContent(const char *fileName, uint8_t *&data, uint32_t &size)
	{
		data = NULL;
		size = 0;

		std::ifstream file(fileName, std::ios::binary);
		if (file.fail() || !file.is_open())
			return false;

		file.seekg(0, std::ios::end);
		size = static_cast<uint32_t>(file.tellg());
		file.seekg(0, std::ios::beg);

		data = new uint8_t[size];
		file.read(reinterpret_cast<char*>(data), size);
		
		file.close();

		return true;
	}
};

#endif

