#ifndef GRAPHICS_ENGINE_FACTORY
#define GRAPHICS_ENGINE_FACTORY

class IGraphicsEngine;

class GraphicsEngineFactory
{
public:
	static IGraphicsEngine* Create();
};

#endif // GRAPHICS_ENGINE_FACTORY

