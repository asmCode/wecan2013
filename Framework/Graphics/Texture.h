#pragma once

#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>

class Texture
{
protected:
	unsigned texId;

	int width;
	int height;
	int bpp;

public:
	enum Wrap
	{
		Wrap_Repeat					= GL_REPEAT,
		Wrap_ClampToEdge			= GL_CLAMP_TO_EDGE
	};

	enum Filter
	{
		Filter_Nearest				= GL_NEAREST,
		Filter_Linear				= GL_LINEAR,
		Filter_LinearMipmapLinear	= GL_LINEAR_MIPMAP_LINEAR
	};

	Texture();
	Texture(
		int width,
		int height,
		int bpp,
		const void *data,
		Wrap wrap,
		Filter minFilter,
		Filter magFilter,
		bool genMipmaps);

	~Texture();

	int GetWidth() const;
	int GetHeight() const;
	int GetBpp() const;

	void BindTexture();
	unsigned GetId() const;
	
	void SetTextureData(const void *data);	
	void SetTextureData(int x, int y, int width, int height, int bpp, const void *data);

	// Bind texture before this method
	void SetWrappingMode(Wrap wrapModel);

	// Bind texture before this method
	void SetFilterModel(Filter filterMinMode, Filter filterMaxMode);

	// Bind texture before this method
	void GenerateMipmaps();
};
