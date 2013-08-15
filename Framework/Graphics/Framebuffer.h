#pragma once

#include <Windows.h>

class Framebuffer
{
private:
	unsigned framebufferId;
	unsigned textureId;
	unsigned depthRenderBufferId;
	unsigned offscreenColorRenderBufferId;

	unsigned prevFramebufferId;

	int width;
	int height;
	int bpp;

public:
	Framebuffer();
	~Framebuffer(void);

	void StartOffscreenRendering();
	void StopOffscreenRendering();

	bool Initialize(int width, int height, int bpp);
	unsigned GetTextureId();

	void BindFramebuffer();
	void Validate();

	void AttachColorTexture(unsigned textureId);
	void AttachDepthTexture(unsigned textureId);

	int GetWidth();
	int GetHeight();
	int GetBpp();

	static void RestoreDefaultFramebuffer();
};
