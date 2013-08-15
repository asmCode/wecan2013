#include "ImageUtils.h"
#include <stddef.h>
#include <memory>
#include <assert.h>

void ImageUtils::FlipVertical(uint8_t *data, uint32_t width, uint32_t height, uint32_t bytesCount)
{
	assert(data != NULL);

	uint32_t lineLength = width * bytesCount;
	uint8_t *tmpData = new uint8_t[lineLength];

	for (uint32_t i = 0; i < height / 2; i++)
	{
		memcpy(tmpData, data + (i * lineLength), lineLength);
		memcpy(data + (i * lineLength), data + ((height - i - 1) * lineLength), lineLength);
		memcpy(data + ((height - i - 1) * lineLength), tmpData, lineLength);
	}

	delete [] tmpData;
}

