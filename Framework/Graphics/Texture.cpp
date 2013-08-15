#include "Texture.h"

#include <assert.h>

Texture::Texture()
{
	this ->width = 0;
	this ->height = 0;
	this ->bpp = 0;
	this ->texId = 0;	
}

Texture::Texture(
	int width,
	int height,
	int bpp,
	const void *data,
	Wrap wrap,
	Filter minFilter,
	Filter magFilter,
	bool genMipmaps)
{	
	this ->width = width;
	this ->height = height;
	this ->bpp = bpp;
	this ->texId = 0;
	
	GLenum format = 0;
	switch (bpp)
	{
		case 32: format = GL_RGBA; break;
		case 24: format = GL_RGB; break;
		case 16: assert(0); break;
		case 8: format = GL_ALPHA; break;
	}
	
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	if (genMipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
	if (texId != 0)
		glDeleteTextures(1, &texId);
}

void Texture::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, texId);
}

unsigned Texture::GetId() const
{
	return texId;
}

int Texture::GetWidth() const
{
	return width;
}

int Texture::GetHeight() const
{
	return height;
}

int Texture::GetBpp() const
{
	return bpp;
}

void Texture::SetTextureData(const void *data)
{
	SetTextureData(0, 0, width, height, bpp, data);
}

void Texture::SetTextureData(int x, int y, int width, int height, int bpp, const void *data)
{
	BindTexture();
	
	GLenum format = 0;
	switch (bpp)
	{
		case 32: format = GL_RGBA; break;
		case 24: format = GL_RGB; break;
		case 16: assert(0); break;
		case 8: format = GL_ALPHA; break;
	}
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, format, GL_UNSIGNED_BYTE, data);
}

void Texture::SetWrappingMode(Texture::Wrap wrapModel)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModel);
}

void Texture::SetFilterModel(Filter filterMinMode, Filter filterMaxMode)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMinMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMaxMode);
}

void Texture::GenerateMipmaps()
{
	glGenerateMipmap(GL_TEXTURE_2D);
}

