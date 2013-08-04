#include "CubeTexture.h"
#include <Graphics/ImageLoader.h>
#include <gl/glew.h>
#include <gl/gl.h>
#include <assert.h>

CubeTexture::CubeTexture(const std::string &texNamePrefix)
{
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, m_texId);

	std::string sideNames[] = {
		"x_pos.jpg",
		"x_neg.jpg",
		"y_neg.jpg",
		"y_pos.jpg",
		"z_neg.jpg",
		"z_pos.jpg"};

	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	uint32_t width;
	uint32_t height;
	uint32_t bytesCount;
	uint8_t *data;

	for (uint32_t i = 0; i < 6; i++)
	{
		if (!ImageLoader::LoadFromFile(texNamePrefix + sideNames[i], data, width, height, bytesCount))
		{
			assert(false);
		}

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT + i,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			data);

		delete [] data;
	}

	glGenerateMipmapEXT(GL_TEXTURE_CUBE_MAP_EXT);

	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, 0);
}

CubeTexture::~CubeTexture(void)
{
	if (m_texId != 0)
	{
		glDeleteTextures(1, &m_texId);
	}
}

uint32_t CubeTexture::GetTextureId() const
{
	return m_texId;
}

