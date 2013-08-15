#pragma once

#include <Math\Matrix.h>
#include <vector>

class Model;

class IScene
{
public:
	~IScene() {};

	virtual void Update(float time, float seconds) = 0;
	virtual void Draw(float time, float seconds) = 0;
	virtual void DrawGlows(float time, float seconds) = 0;
	virtual void DrawOpacities(float time, float seconds) {};

	virtual void SetVisibility(bool visible) = 0;

	virtual bool IsActiveOnTime(float time) = 0;

	virtual std::vector<Model*>& GetModels() = 0;

	virtual void GetLightTransform(sm::Matrix &lightTransform) {}
};

