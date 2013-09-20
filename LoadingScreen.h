#pragma once

#include <string>
#include <Graphics/SpriteBatch.h>
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

	SpriteBatch *m_spriteBatch;

public:
	LoadingScreen(void);
	~LoadingScreen(void);

	void Initialize();
	void Initialize(const std::string &basePath);
	void Release();

	void SetStepsCount(int steps);
	void ProgressStep();

	void Update(float time, float ms);
	void Draw();
};
