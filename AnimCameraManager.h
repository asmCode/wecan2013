#pragma once

#include <string>
#include <vector>
#include "GraphicsLibrary/AnimCamera.h"
#include <Graphics/Animation.h>

class AnimCameraComparer
{
public:
	bool operator() (AnimCamera *c1, AnimCamera *c2)
	{
		// to nie wystarczy, bo animacja fov lub dist moze byc dluzsza od pos/rot/scale.
		// no ale nie ma co srac w majtasy, to powinny wystarczyc
		return c1->GetLastKeyFrameTime() < c2->GetLastKeyFrameTime();
	}
};

class AnimCameraManager
{
private:
	std::vector<AnimCamera*> cameras;

	unsigned camIndex;

public:
	AnimCameraManager(void);
	~AnimCameraManager(void);

	void Load(const std::string &path, Animation *anim);

	// nie mozna cofac czasu!
	AnimCamera *GetActiveCamera(float time);
};
