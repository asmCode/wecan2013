#include "DepthTexture.h"
#include <GL/glew.h>
#include <gl/gl.h>
#include <stddef.h>

DepthTexture::DepthTexture(uint32_t width, uint32_t height)
{
	m_width = width;
	m_height = height;

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

DepthTexture::~DepthTexture()
{
	if (m_id != 0)
		glDeleteTextures(1, &m_id);
}

uint32_t DepthTexture::GetWidth() const
{
	return m_width;
}

uint32_t DepthTexture::GetHeight() const
{
	return m_height;
}

void DepthTexture::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

unsigned DepthTexture::GetId()
{
	return m_id;
}

void DepthTexture::SetTextureData(const uint8_t *data)
{
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_DEPTH_COMPONENT, GL_FLOAT, data);

	glBindTexture(GL_TEXTURE_2D, 0);
}
