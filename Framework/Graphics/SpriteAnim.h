#pragma once

#include "IUpdateable.h"
#include "TexPart.h"

class Texture;

class SpriteAnim : public IUpdateable
{
private:
	Texture *tex;
	int frameWidth;
	int frameHeight;
	int framesCount;
	float frameDuration;
	float currentTime;
	TexPart *framesTexPart;
	
	int currentFrame;
	
public:
	SpriteAnim(Texture *tex,
			   int frameWidth,
			   int frameHeight,
			   int columnsCount,
			   int framesCount);
	~SpriteAnim();
	
	void SetFrameDuration(float duration);
	void SetAnimationDuration(float duration);
	void SetFrame(unsigned frameIndex);
	void Update(float time, float seconds);
	Texture* GetTexture() const;
	const float* GetCurrentCoords() const;
	const float* GetCoordsAtTime(float time) const;
	const TexPart& GetTexPartAtTime(float time) const;
};
