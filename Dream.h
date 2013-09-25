#pragma once

#include <string>
#include <Graphics/SpriteBatch.h>
#include <Graphics/Interpolators/LinearInterpolator.h>
#include <Math\Matrix.h>

class Texture;

class Dream
{
private:
	Texture **t;
	float currentT;
	float tPerSec;

	int scrWidth;
	int scrHeight;

	LinearInterpolator<float> a;

	SpriteBatch *m_spriteBatch;

public:

	static float start;
	static float end;

	Dream(void);
	~Dream(void);

	void Initialize();
	void Release();

	void Update(float time, float ms);
	void Draw(float time);
};
