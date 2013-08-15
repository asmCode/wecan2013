#pragma once

#include <string>
#include <Math\Matrix.h>

class Texture;

class LoadingScreen
{
private:
	Texture *loadingTex;
	int steps;
	int progressStep;

	int scrWidth;
	int scrHeight;

public:
	LoadingScreen(void);
	~LoadingScreen(void);

	void Initialize();
	void Initialize(const std::string &basePath);
	void Release();

	void SetStepsCount(int steps);
	void ProgressStep();

	void Update(float time, float ms);
	void Draw(float time, float ms, const sm::Matrix &view, const sm::Matrix &proj, const sm::Vec3 &camPos);
};
