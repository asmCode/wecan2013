#include "SpriteAnim.h"
#include "Texture.h"
#include <stddef.h>
#include <assert.h>

SpriteAnim::SpriteAnim(Texture *tex,
		   int frameWidth,
		   int frameHeight,
		   int columnsCount,
		   int framesCount)
{
	this ->tex = tex;
	this ->frameWidth = frameWidth;
	this ->frameHeight = frameHeight;
	this ->framesCount = framesCount;
	
	this ->frameDuration = 1.0f; // 1 second by default
	this ->currentTime = 0.0;
	
	this ->framesTexPart = new TexPart[framesCount];
	this ->currentFrame = 0;
	
	int texWidth = tex ->GetWidth();
	int texHeight = tex ->GetHeight();
	assert((texWidth >= columnsCount * frameWidth) && "frames count is too high");
	
	int xShift = 0;
	int yShift = 0;
	int frameIndex = 0;
		
	while (framesCount--)
	{
		framesTexPart[frameIndex] = TexPart(tex, sm::Rect<int>(xShift, yShift, frameWidth, frameHeight));
		frameIndex++;
		
		xShift += frameWidth;
		if (xShift == columnsCount * frameWidth)
		{
			xShift = 0;
			yShift += frameHeight;
		}
		
		assert(yShift < texHeight && "to high frames count");
	}
}

SpriteAnim::~SpriteAnim()
{
	if (framesTexPart != NULL)
		delete [] framesTexPart;
}

void SpriteAnim::SetFrameDuration(float duration)
{
	frameDuration = duration;
}

void SpriteAnim::SetAnimationDuration(float duration)
{
	if (framesCount == 0)
		return;
	
	frameDuration = duration / (float)framesCount;
}

void SpriteAnim::SetFrame(unsigned frameIndex)
{
	currentFrame = frameIndex;
	currentTime = frameDuration * currentFrame;
}

void SpriteAnim::Update(float time, float seconds)
{
	currentTime += seconds;
	currentFrame = (int)(currentTime / frameDuration);
	if (currentFrame >= framesCount)
	{
		currentFrame = 0;
		currentTime = 0.0f;
	}
}

Texture* SpriteAnim::GetTexture() const
{
	return tex;
}

const float* SpriteAnim::GetCurrentCoords() const
{
	return framesTexPart[currentFrame].TexCoords;
}

const float* SpriteAnim::GetCoordsAtTime(float time) const
{
	int frameIndex = (int)(time / frameDuration);
	if (frameIndex >= framesCount)
		frameIndex = framesCount - 1;
	
	return framesTexPart[frameIndex].TexCoords;
}

const TexPart& SpriteAnim::GetTexPartAtTime(float time) const
{
	int frameIndex = (int)(time / frameDuration);
	if (frameIndex >= framesCount)
		frameIndex = framesCount - 1;
	
	return framesTexPart[frameIndex];	
}