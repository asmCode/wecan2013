#pragma once

#include <stdint.h>

class Framebuffer
{
private:
	uint32_t framebufferId;
	uint32_t textureId;
	uint32_t depthRenderBufferId;
	uint32_t offscreenColorRenderBufferId;

	uint32_t prevFramebufferId;

	int width;
	int height;
	int bpp;

public:
	Framebuffer();
	~Framebuffer(void);

	void StartOffscreenRendering();
	void StopOffscreenRendering();

	bool Initialize(int width, int height, int bpp);
	uint32_t GetTextureId();

	void BindFramebuffer();
	void Validate();

	void AttachColorTexture(uint32_t textureId);
	void AttachDepthTexture(uint32_t textureId);

	int GetWidth();
	int GetHeight();
	int GetBpp();

	static void RestoreDefaultFramebuffer();
};
