#pragma once

#include "Point.h"
#include "FontLetter.h"
#include <string>
#include "Color.h"

class SpriteBatch;

class FontRenderer
{
private:
	FontLetter texLetters[256];
	SpriteBatch *m_spriteBatch;
	
	FontRenderer();
	static bool ParseBounds(const std::string &strBounds, sm::Rect<int> &bounds);
	static Texture *LoadFontBitmap(const std::string &path);
	
public:
	static FontRenderer* LoadFromFile(const char *path, SpriteBatch *spriteBatch);
	~FontRenderer();
	
	void DrawString(const char *text,
					unsigned x,
					unsigned y,
					const Color &color);
	
	sm::Point<int> MeasureString(const char *text);
};

