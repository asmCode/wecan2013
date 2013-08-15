#include "Blur.h"
#include "GraphicsLibrary/Framebuffer.h"
#include <Graphics/Shader.h>
#include "GraphicsLibrary/Texture.h"
#include <Math/Matrix.h>

Blur::Blur(int blurCount, Shader *blurEffect, int width, int height)
{
	this ->blurEffect = blurEffect;
	this ->blurCount = blurCount;

	this ->width = width;
	this ->height = height;
	this ->bpp = 32;

	framebuffer = new Framebuffer();
	framebuffer ->Initialize(width, height, bpp);

	tmpTex = new Texture(width, height, bpp, NULL, Texture::Wrap_ClampToEdge, Texture::Filter_Linear, Texture::Filter_Linear, false);

	blurredTextures = new Texture * [blurCount];
	for (int i = 0; i < blurCount; i++)
		blurredTextures[i] = new Texture(width, height, bpp, NULL, Texture::Wrap_ClampToEdge, Texture::Filter_Linear, Texture::Filter_Linear, false);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Blur::~Blur(void)
{
	for (int i = 0; i < blurCount; i++)
		delete blurredTextures[i];

	delete [] blurredTextures;

	if (tmpTex != NULL)
		delete tmpTex;

	delete framebuffer;
}

void Blur::BlurTexture(UINT srcTex, UINT dstTex, bool glow)
{
	framebuffer ->BindFramebuffer();
	framebuffer ->AttachColorTexture(dstTex);
	framebuffer ->Validate();

	glBindTexture(GL_TEXTURE_2D, 0);
	blurEffect ->SetTextureParameter("texToBlur", 0, srcTex);

	//blurEffect ->BeginPass(0);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 1); glVertex2i(0, 0);
	glTexCoord2i(1, 1); glVertex2i(width, 0);
	glTexCoord2i(1, 0); glVertex2i(width, height);
	glTexCoord2i(0, 0); glVertex2i(0, height);
	glEnd();

	//blurEffect ->EndPass(0);

	Framebuffer::RestoreDefaultFramebuffer();
}

void Blur::MakeBlur(UINT texIdToBlur, int sampleStep, bool glow)
{
	glDepthMask(false);

	glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_CURRENT_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glColor4f(1, 1, 1, 1);

	glViewport(0, 0, width, height);

	sm::Matrix mvp = sm::Matrix::Ortho2DMatrix(0, (float)width, 0, (float)height);
	//blurEffect ->SetParameter("mvp", mvp);

	for (int i = 0; i < blurCount; i++)
	{
//		blurEffect ->SetParameter("scanStep", (float)sampleStep / width, 0);
		BlurTexture(texIdToBlur, tmpTex ->GetId(), glow);

		//blurEffect ->SetParameter("scanStep", 0, (float)sampleStep / height);
		BlurTexture(tmpTex ->GetId(), blurredTextures[i] ->GetId(), glow);

		texIdToBlur = blurredTextures[i] ->GetId();
	}

	glPopAttrib();

	glDepthMask(true);
}

UINT Blur::GetBlurredTexture(int index)
{
	return blurredTextures[index] ->GetId();
}
