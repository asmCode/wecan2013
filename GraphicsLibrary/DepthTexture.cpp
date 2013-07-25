#include "DepthTexture.h"
#include <GL/glew.h>
#include <gl/gl.h>
#include <stddef.h>

DepthTexture::DepthTexture(int width, int height)
{
	this ->width = width;
	this ->height = height;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	uint32_t err = 0;
	err = glGetError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32_ARB,
		width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	err = glGetError();

	glBindTexture(GL_TEXTURE_2D, 0);
}

DepthTexture::~DepthTexture()
{
	if (id != NULL)
		glDeleteTextures(1, &id);
}

int DepthTexture::GetWidth() const
{
	return width;
}

int DepthTexture::GetHeight() const
{
	return height;
}

void DepthTexture::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

uint32_t DepthTexture::GetId()
{
	return id;
}

void DepthTexture::SetTextureData(const uint8_t *data)
{
	glBindTexture(GL_TEXTURE_2D, id);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, data);

	glBindTexture(GL_TEXTURE_2D, 0);
}
