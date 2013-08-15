#include "DepthTexture.h"

DepthTexture::DepthTexture(int width, int height)
{
	this ->width = width;
	this ->height = height;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	unsigned err = 0;
	err = glGetError();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
		width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	err = glGetError();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/*glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL );*/


//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	err = glGetError();

	glBindTexture(GL_TEXTURE_2D, 0);
}

DepthTexture::~DepthTexture()
{
	if (id != 0)
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

unsigned DepthTexture::GetId()
{
	return id;
}

void DepthTexture::SetTextureData(const unsigned char *data)
{
	glBindTexture(GL_TEXTURE_2D, id);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, data);

	glBindTexture(GL_TEXTURE_2D, 0);
}
