#include "GraphicsEngineWin.h"
#include "ImageLoader.h"
#include "ModelLoader.h"
#include "AnimationLoader.h"
#include "MaterialLoader.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include <assert.h>

GraphicsEngineWin::GraphicsEngineWin(void)
{
}

GraphicsEngineWin::~GraphicsEngineWin(void)
{
}

Texture* GraphicsEngineWin::LoadTexture(const std::string &path)
{
	return TextureLoader::Load(path);
}

Shader* GraphicsEngineWin::LoadShader(const std::string &vertexShaderPath,
								      const std::string &fragmentShaderPath)
{
	Shader *shader = Shader::LoadFromFile(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

	return shader;
}

Model* GraphicsEngineWin::LoadModel(const std::string &path)
{
	return ModelLoader::LoadFromFile(path);
}

Animation* GraphicsEngineWin::LoadAnimation(const std::string &path)
{
	return AnimationLoader::LoadFromFile(path);
}

Material* GraphicsEngineWin::LoadMaterial(const std::string &path)
{
	return MaterialLoader::LoadFromFile(path);
}

void GraphicsEngineWin::DrawSprite(Texture *texture, uint32_t x, uint32_t y)
{
	assert(texture != NULL);
}

//void GraphicsEngineWin::DrawImage(Image *image, float x, float y)
//{
//	//image-
//}
//
