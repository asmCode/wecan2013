#pragma once

#include <Windows.h>

class Texture;
class Shader;
class Framebuffer;

class Blur
{
private:
	int blurCount;

	Shader *blurEffect;
	Framebuffer *framebuffer;

	Texture **blurredTextures;
	Texture *tmpTex;

	void BlurTexture(UINT srcTex, UINT dstTex, bool glow);

	int width;
	int height;
	int bpp;

public:
	Blur(int blurCount, Shader *blurEffect, int width, int height);
	~Blur(void);

	void MakeBlur(UINT texIdToBlur, int sampleStep, bool glow);

	UINT GetBlurredTexture(int index);
};
