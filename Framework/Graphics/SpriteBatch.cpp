#include "SpriteBatch.h"
#include "../Math/Vec2.h"
#include "../Math/Vec3.h"
#include "Shader.h"

#include <Windows.h>
#include <gl/gl.h>
#include <memory.h>
#include <assert.h>

const float SpriteBatch::Verts[8] = {
	-0.5f, -0.5f,	
	 0.5f, -0.5f,
	-0.5f,  0.5f,
	 0.5f,  0.5f
};

const float SpriteBatch::Coords[8] = {
	0, 0,	
	1, 0,
	0, 1,
	1, 1
};

const unsigned char SpriteBatch::ColorMask[16] = {
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
	255, 255, 255, 255
};

SpriteBatch::SpriteBatch(Shader *shader, const sm::Matrix &mvp) :
	m_shader(shader),
	m_mvp(mvp),
	m_isDepth(false),
	m_isBlend(false)
{
	assert(m_shader != NULL);
	
	m_shader->BindVertexChannel(0, "a_position");
	m_shader->BindVertexChannel(1, "a_coords");
	m_shader->LinkProgram();
}

void SpriteBatch::Begin()
{
	m_shader->UseProgram();
	m_shader->SetMatrixParameter("u_mvp", m_mvp);
	m_shader->SetParameter("u_color", 1, 1, 1, 1);
	
	m_isDepth = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
	m_isBlend = glIsEnabled(GL_BLEND) == GL_TRUE;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if (m_isDepth) glDisable(GL_DEPTH_TEST);
	if (!m_isBlend) glEnable(GL_BLEND);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // coords
}

void SpriteBatch::End()
{
	if (m_isDepth) glEnable(GL_DEPTH_TEST);
	if (!m_isBlend) glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void SpriteBatch::Draw(const Color &color, int x, int y, int width, int height)
{
	float verts[8];
	CreateQuad(verts, x, y, width, height);
	
	const unsigned char _color[16] = {
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A
	};	
	
	Draw(NULL, verts, NULL, _color);
}

void SpriteBatch::Draw(Texture *tex, int x, int y, int width, int height)
{
	float verts[8];
	CreateQuad(verts, x, y, width, height);
	
	Draw(tex, verts, Coords, NULL);	
}

void SpriteBatch::Draw(Texture *tex, int x, int y)
{
	Draw(tex, x, y, tex ->GetWidth(), tex ->GetHeight());
}

void SpriteBatch::Draw(Texture *tex, const float *texCoords, int x, int y, int width, int height)
{
	float verts[8];
	CreateQuad(verts, x, y, width, height);
	
	Draw(tex, verts, texCoords, NULL);
}

void SpriteBatch::Draw(const TexPart &texPart, int x, int y)
{
	Draw(texPart, x, y, texPart.ImageRect.Width, texPart.ImageRect.Height);
}

void SpriteBatch::Draw(const TexPart &texPart, int x, int y, int width, int height)
{
	float verts[8];
	CreateQuad(verts, x, y, width, height);
	
	Draw(texPart.Tex, verts, texPart.TexCoords, NULL);
}

void SpriteBatch::Draw(const TexPart &texPart, const Color &colorMask, int x, int y)
{
	Draw(texPart, colorMask, x, y, texPart.ImageRect.Width, texPart.ImageRect.Height);
}

void SpriteBatch::Draw(const TexPart &texPart, const Color &colorMask, int x, int y, int width, int height)
{
	float verts[8];
	CreateQuad(verts, x, y, width, height);
	
	const unsigned char _color[16] = {
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A
	};
	
	Draw(texPart.Tex, verts, texPart.TexCoords, _color);
}

void SpriteBatch::Draw(const Color &color, sm::Matrix trans)
{
	sm::Vec3 corners[4] = {
		sm::Vec3(-0.5f, -0.5f, 0.0f),
		sm::Vec3( 0.5f, -0.5f, 0.0f),
		sm::Vec3(-0.5f,  0.5f, 0.0f),
		sm::Vec3( 0.5f,  0.5f, 0.0f)
	};
	
	for (unsigned i = 0; i < 4; i++)
		corners[i] = trans * corners[i];
	
	float verts[8] = {
		corners[0].x, corners[0].y,
		corners[1].x, corners[1].y,
		corners[2].x, corners[2].y,
		corners[3].x, corners[3].y
	};
	
	const unsigned char _color[16] = {
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A,
		color.R, color.G, color.B, color.A
	};
	
	Draw(NULL, verts, NULL, _color);
}

void SpriteBatch::Draw(const TexPart &texPart, const Color &colorMask, const sm::Matrix &trans)
{
	sm::Vec3 corners[4] = {
		sm::Vec3(-0.5f, -0.5f, 0.0f),
		sm::Vec3( 0.5f, -0.5f, 0.0f),
		sm::Vec3(-0.5f,  0.5f, 0.0f),
		sm::Vec3( 0.5f,  0.5f, 0.0f)
	};
	
	for (unsigned i = 0; i < 4; i++)
		corners[i] = trans * corners[i];
	
	float verts[8] = {
		corners[0].x, corners[0].y,
		corners[1].x, corners[1].y,
		corners[2].x, corners[2].y,
		corners[3].x, corners[3].y
	};
	
	const unsigned char _color[16] = {
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A,
		colorMask.R, colorMask.G, colorMask.B, colorMask.A
	};
	
	Draw(texPart.Tex, verts, texPart.TexCoords, _color);
}

// TODO: should be static!
void SpriteBatch::Draw(Texture *tex,
		  const float *verts,
		  const float *coords,
		  const unsigned char *colorMask)
{
	m_shader->SetTextureParameter("u_tex", 0, tex->GetId());

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, verts);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, coords);
	
	/*if (colorMask != NULL)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorMask);
	}*/
	
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void SpriteBatch::CreateQuad(float *vertices, int x, int y, int width, int height)
{
	vertices[0] = static_cast<float>(x);
	vertices[1] = static_cast<float>(y + height);
	vertices[2] = static_cast<float>(x + width);
	vertices[3] = static_cast<float>(y + height);
	vertices[4] = static_cast<float>(x);
	vertices[5] = static_cast<float>(y);
	vertices[6] = static_cast<float>(x + width);
	vertices[7] = static_cast<float>(y);
}

