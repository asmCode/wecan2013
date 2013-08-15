#pragma once

#include "Texture.h"
#include "Rect.h"

class TexPart
{
public:
	Texture *Tex;
	float TexCoords[8];
	sm::Rect<int> ImageRect;

	TexPart();
	TexPart(Texture *tex);
	TexPart(Texture *tex, const sm::Rect<int> &texRect);
};