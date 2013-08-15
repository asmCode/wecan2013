#include "PVRTexture.h"
#include <assert.h>
#include <math.h>

PVRTexture::PVRTexture(unsigned width,
					   unsigned height,
					   unsigned bpp,
					   GLenum internalFormat,
					   std::vector<std::string> data)
{
	if (data.size() == 0)
		return;
	
	this ->width = width;
	this ->height = height;
	this ->bpp = bpp;
	
	GLenum err;
	
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	
	if (data.size() > 1)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
						
	for (unsigned i = 0; i < data.size(); i++)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D,
							   i, internalFormat, width, height, 0,
							   data[i].size(), data[i].c_str());
		
		err = glGetError();
		if (err != GL_NO_ERROR)
			assert(0);
		
		width = fmaxf(width >> 1, 1);
		height = fmaxf(height >> 1, 1);
	}
}

PVRTexture::~PVRTexture()
{
}

void PVRTexture::SetTextureData(const unsigned char *data)
{
	assert(0 && "uniplemented");
}
