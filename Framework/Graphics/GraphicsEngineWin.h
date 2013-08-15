#ifndef GRAPHICS_ENGINE_WIN
#define GRAPHICS_ENGINE_WIN

#include "IGraphicsEngine.h"
#include <stdint.h>

class GraphicsEngineWin : public IGraphicsEngine
{
public:
	GraphicsEngineWin(void);
	~GraphicsEngineWin(void);

	Texture* LoadTexture(const std::string &path); 

	Shader* LoadShader(const std::string &vertexShaderPath,
					   const std::string &fragmentShaderPath);

	Model* LoadModel(const std::string &path);

	Animation* LoadAnimation(const std::string &path);

	Material* LoadMaterial(const std::string &path);

	void DrawSprite(Texture *texture, uint32_t x, uint32_t y);

	//void DrawImage(Image *image, float x, float y);
};

#endif // GRAPHICS_ENGINE_WIN

