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
		glDeleteRenderbuffers(1, &depthRenderBufferId);

	if (offscreenColorRenderBufferId != 0)
		glDeleteRenderbuffers(1, &offscreenColorRenderBufferId);

	if (framebufferId != 0)
		glDeleteFramebuffers(1, &framebufferId);
}

bool Framebuffer::Initialize(int width, int height, int bpp)
{
	this ->width = width;
	this ->height = height;
	this ->bpp = bpp;

	// depth renderbuffer
	glGenRenderbuffers(1, &depthRenderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// offscreen color renderbuffer
	glGenRenderbuffers(1, &offscreenColorRenderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, offscreenColorRenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// framebuffer
	glGenFramebuffers(1, &framebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void Framebuffer::Validate()
{
	unsigned status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(GL_FRAMEBUFFER_COMPLETE == status);

	//#define GL_FRAMEBUFFER_COMPLETE_EXT       0x8CD5
	//#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
	//#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT 0x8CD7
	//#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT 0x8CD9
	//#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT 0x8CDA
	//#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT 0x8CDB
	//#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT 0x8CDC
	//#define GL_FRAMEBUFFER_UNSUPPORTED_EXT    0x8CDD
}

unsigned Framebuffer::GetTextureId()
{
	return textureId;
}

void Framebuffer::AttachColorTexture(unsigned textureId, uint32_t index)
{
	if (index == 0)
		this ->textureId = textureId;

	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, textureId, 0);
}

void Framebuffer::DettachColorTexture(uint32_t index)
{
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, 0, 0);
}

void Framebuffer::AttachDepthTexture(unsigned textureId)
{
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
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
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
}

void Framebuffer::RestoreDefaultFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
