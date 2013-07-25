#include "Framebuffer.h"
#include <GL/glew.h>
#include <gl/gl.h>
#include <assert.h>

Framebuffer::Framebuffer()
{
	textureId = 0;
	framebufferId = 0;
	depthRenderBufferId = 0;
}

Framebuffer::~Framebuffer(void)
{
	if (depthRenderBufferId != 0)
		glDeleteRenderbuffersEXT(1, &depthRenderBufferId);

	if (offscreenColorRenderBufferId != 0)
		glDeleteRenderbuffersEXT(1, &offscreenColorRenderBufferId);

	if (framebufferId != 0)
		glDeleteFramebuffersEXT(1, &framebufferId);
}

bool Framebuffer::Initialize(int width, int height, int bpp)
{
	this ->width = width;
	this ->height = height;
	this ->bpp = bpp;

	// depth renderbuffer
	glGenRenderbuffersEXT(1, &depthRenderBufferId);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderBufferId);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// offscreen color renderbuffer
	glGenRenderbuffersEXT(1, &offscreenColorRenderBufferId);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, offscreenColorRenderBufferId);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, width, height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	// framebuffer
	glGenFramebuffersEXT(1, &framebufferId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferId);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthRenderBufferId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	return true;
}

void Framebuffer::Validate()
{
	uint32_t status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	assert(GL_FRAMEBUFFER_COMPLETE_EXT == status);

	//#define GL_FRAMEBUFFER_COMPLETE_EXT       0x8CD5
	//#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
	//#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT 0x8CD7
	//#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT 0x8CD9
	//#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT 0x8CDA
	//#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT 0x8CDB
	//#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT 0x8CDC
	//#define GL_FRAMEBUFFER_UNSUPPORTED_EXT    0x8CDD
}

uint32_t Framebuffer::GetTextureId()
{
	return textureId;
}

void Framebuffer::AttachColorTexture(uint32_t textureId)
{
	this ->textureId = textureId;

	glFramebufferTexture2DEXT(
		GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureId, 0);
}

void Framebuffer::AttachDepthTexture(uint32_t textureId)
{
	glFramebufferTexture2DEXT(
		GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, textureId, 0);
}

int Framebuffer::GetWidth()
{
	return width;
}

int Framebuffer::GetHeight()
{
	return height;
}

int Framebuffer::GetBpp()
{
	return bpp;
}

void Framebuffer::BindFramebuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferId);
}

void Framebuffer::RestoreDefaultFramebuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
