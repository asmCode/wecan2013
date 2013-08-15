#include "GraphicsEngineFactory.h"
#include "GraphicsEngineWin.h"

IGraphicsEngine* GraphicsEngineFactory::Create()
{
	return new GraphicsEngineWin();
}

