#include "Blur.h"
#include <Graphics/Framebuffer.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <Math/Matrix.h>

#include <Graphics/SpriteBatch.h>

Blur::Blur(int blurCount, Shader *horiBlurShader, Shader *vertBlurShader, int width, int height) :
	m_horiBlurShader(horiBlurShader),
	m_vertBlurShader(vertBlurShader)
{
	this ->blurCount = blurCount;

	this ->width = width;
	this ->height = height;
	this ->bpp = 32;

	CreateQuad(width, height);

	glGenBuffers(1, &m_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), m_quadVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

void Blur::BlurTexture(uint32_t srcTex, uint32_t dstTex, bool glow)
{
}

void Blur::MakeBlur(uint32_t texIdToBlur)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(8));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

	framebuffer ->BindFramebuffer();

	glDepthMask(false);
	glColorMask(true, true, true, true);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	glViewport(0, 0, width, height);
	
	for (int i = 0; i < blurCount; i++)
	{
		// horizontal blur
		framebuffer ->AttachColorTexture(tmpTex->GetId());
		framebuffer ->Validate();

		m_horiBlurShader->UseProgram();
		m_horiBlurShader->SetMatrixParameter("u_mvp", sm::Matrix::Ortho2DMatrix(0, width, 0, height));
		m_horiBlurShader->SetParameter("u_pixelSize", 1.0f / static_cast<float>(width));
		m_horiBlurShader->SetTextureParameter("u_tex", 0, texIdToBlur);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//

		// vertical blur
		framebuffer ->AttachColorTexture(blurredTextures[i] ->GetId());
		framebuffer ->Validate();

		m_vertBlurShader->UseProgram();
		m_vertBlurShader->SetMatrixParameter("u_mvp", sm::Matrix::Ortho2DMatrix(0, width, 0, height));
		m_vertBlurShader->SetParameter("u_pixelSize", 1.0f / static_cast<float>(height));
		m_vertBlurShader->SetTextureParameter("u_tex", 0, tmpTex ->GetId());

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//

		texIdToBlur = blurredTextures[i] ->GetId();
	}

	Framebuffer::RestoreDefaultFramebuffer();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Texture* Blur::GetBlurredTexture(int index)
{
	return blurredTextures[index];
}

void Blur::CreateQuad(int width, int height)
{
	//  1 0
	//  0 0
	m_quadVerts[0] = static_cast<float>(0);
	m_quadVerts[1] = static_cast<float>(height);
	m_quadVerts[2] = 0.0f;
	m_quadVerts[3] = 1.0f;

	//  0 0
	//  1 0
	m_quadVerts[4] = static_cast<float>(0);
	m_quadVerts[5] = static_cast<float>(0);
	m_quadVerts[6] = 0.0f;
	m_quadVerts[7] = 0.0f;

	//  0 1
	//  0 0
	m_quadVerts[8] = static_cast<float>(width);
	m_quadVerts[9] = static_cast<float>(height);
	m_quadVerts[10] = 1.0f;
	m_quadVerts[11] = 1.0f;

	//  0 0
	//  0 1
	m_quadVerts[12] = static_cast<float>(width);
	m_quadVerts[13] = static_cast<float>(0);
	m_quadVerts[14] = 1.0f;
	m_quadVerts[15] = 0.0f;
}

