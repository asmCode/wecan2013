#pragma once

#include <stdint.h>

class DepthTexture
{
private:
	uint32_t id;
	int width;
	int height;

public:
	DepthTexture(int width, int height);
	~DepthTexture();

	int GetWidth() const;
	int GetHeight() const;

	void BindTexture();
	uint32_t GetId();
	void SetTextureData(const uint8_t *data);
};
