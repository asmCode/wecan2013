#include "Image.h"

uint32_t Image::GetWidth() const
{
	return m_width;
}

uint32_t Image::GetHeight() const
{
	return m_height;
}

uint8_t Image::GetBytesPerColor() const
{
	return m_bytesPerColor;
}

