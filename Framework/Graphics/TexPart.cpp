#include "TexPart.h"
#include <string.h>

TexPart::TexPart() : ImageRect(0, 0, 0, 0)
{
	Tex = NULL;
	memset(TexCoords, 0, sizeof(float) * 8);
}

TexPart::TexPart(Texture *tex) : ImageRect(0, 0, tex ->GetWidth(), tex ->GetHeight())
{
	this ->Tex = tex;
	
	TexCoords[0] = 0;
	TexCoords[1] = 0;
	
	TexCoords[2] = 1;
	TexCoords[3] = 0;
	
	TexCoords[4] = 0;
	TexCoords[5] = 1;
	
	TexCoords[6] = 1;
	TexCoords[7] = 1;
}

TexPart::TexPart(Texture *tex, const sm::Rect<int> &texRect) : ImageRect(texRect)
{
	this ->Tex = tex;
	
	float x1 = (float)texRect.X / (float)tex ->GetWidth();
	float y1 = ((float)texRect.Y / (float)tex ->GetHeight());
	float x2 = (float)(texRect.X  + texRect.Width) / (float)tex ->GetWidth();
	float y2 = ((float)(texRect.Y  + texRect.Height) / (float)tex ->GetHeight());
	
	TexCoords[0] = x1;
	TexCoords[1] = y2;
	
	TexCoords[2] = x1;
	TexCoords[3] = y1;
	
	TexCoords[4] = x2;
	TexCoords[5] = y2;
	
	TexCoords[6] = x2;
	TexCoords[7] = y1;
}
