#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <string>
#include <stdint.h>

class ImageLoader
{
public:
	static bool LoadFromFile(const std::string &path,
							 uint8_t *&data,
							 uint32_t &width,
							 uint32_t &height,
							 uint32_t &bytesCount);
};

#endif

