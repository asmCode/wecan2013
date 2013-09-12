#pragma once

#include <stdint.h>

class DepthTexture
{
private:
	uint32_t m_id;
	uint32_t m_width;
	uint32_t m_height;

public:
	DepthTexture(uint32_t width, uint32_t height);
	~DepthTexture();

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;

	void BindTexture();
	uint32_t GetId();
	void SetTextureData(const uint8_t *data);
};
