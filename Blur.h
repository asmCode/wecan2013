#pragma once

#include <stdint.h>

class Texture;
class Shader;
class Framebuffer;

class Blur
{
private:
	int blurCount;

	Shader *m_vertBlurShader;
	Shader *m_horiBlurShader;
	Framebuffer *framebuffer;

	Texture **blurredTextures;
	Texture *tmpTex;

	void BlurTexture(uint32_t srcTex, uint32_t dstTex, bool glow);

	int width;
	int height;
	int bpp;

	uint32_t m_quadVBO;
	float m_quadVerts[16];

	void CreateQuad(int width, int height);

public:
	Blur(int blurCount, Shader *horiBlurShader, Shader *vertBlurShader, int width, int height);
	~Blur(void);

	void MakeBlur(uint32_t texIdToBlur);

	Texture* GetBlurredTexture(int index);
};
