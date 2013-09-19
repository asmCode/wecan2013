#pragma once

#include <vector>
#include <stdint.h>

class MeshPart;

class IAnimationClip
{
public:
	virtual ~IAnimationClip() {}

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(float time, float seconds) = 0;

	virtual float GetLastKeyframeTime() = 0;

	virtual void GetMeshParts(std::vector<MeshPart*> &meshParts) = 0;
};

