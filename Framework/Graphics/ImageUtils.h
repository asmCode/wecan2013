#ifndef IMAGE_UTILS
#define IMAGE_UTILS

#include <stdint.h>

class ImageUtils
{
public:
	static void FlipVertical(uint8_t *data, uint32_t width, uint32_t height, uint32_t bytesCount);
};

#endif // IMAGE_UTILS

