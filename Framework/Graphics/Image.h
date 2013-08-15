#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

class Image
{
public:
	virtual ~Image() {};

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
	uint8_t GetBytesPerColor() const;

	virtual void Draw() = 0;

protected:
	Image();

private:
	uint32_t m_width;
	uint32_t m_height;
	uint8_t m_bytesPerColor;
};

#endif

