#pragma once

#include "Texture.h"
#include "TexPart.h"
#include "../Math/Matrix.h"
#include "../Math/Vec3.h"
#include "Color.h"

class Shader;

class SpriteBatch
{	
public:
	SpriteBatch(Shader *shader, const sm::Matrix &mvp);

	void Begin();
	void End();
	
	void Draw(const Color &color, int x, int y, int width, int height);
	void Draw(Texture *tex, int x, int y, int width, int height);
	void Draw(Texture *tex, int x, int y);
	void Draw(Texture *tex, const float *texCoords, int x, int y, int width, int height);
	void Draw(const TexPart &texPart, int x, int y);
	void Draw(const TexPart &texPart, int x, int y, int width, int height);
	void Draw(const TexPart &texPart, const Color &colorMask, int x, int y);
	void Draw(const TexPart &texPart, const Color &colorMask, int x, int y, int width, int height);
	
	void Draw(const Color &color, sm::Matrix trans);
	void Draw(const TexPart &texPart, const Color &colorMask, const sm::Matrix &trans);
	
	void Draw(
		Texture *tex,
		const float *verts,
		const float *coords,
		const unsigned char *colorMask);

private:
	Shader *m_shader;

	sm::Matrix m_mvp;

	bool m_isDepth;
	bool m_isBlend;
	
	static const float Verts[8];
	static const float Coords[8];
	static const unsigned char ColorMask[16];

	void CreateQuad(float *vertices, int x, int y, int width, int height);
};
