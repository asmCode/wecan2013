#include <stddef.h>
#include "FontRenderer.h"

#include <Graphics/ImageLoader.h>
#include <IO/Path.h>
#include <XML/XMLNode.h>
#include <XML/XMLLoader.h>
#include "SpriteBatch.h"

#include <Windows.h>
#include <gl/gl.h>

FontRenderer::FontRenderer() :
	m_spriteBatch(NULL)
{
}

Texture *FontRenderer::LoadFontBitmap(const std::string &path)
{
	if (!Path::IsFileExists(path.c_str()))
		return NULL;
	
	uint8_t *data;
	uint32_t width;
	uint32_t height;
	uint32_t bytesCount;

	if (!ImageLoader::LoadFromFile(path, data, width, height, bytesCount))
		return NULL;
	
	Texture *texture = new Texture(
		width,
		height,
		bytesCount * 8,
		data,
		Texture::Wrap_ClampToEdge,
		Texture::Filter_Nearest,
		Texture::Filter_Nearest,
		false);

	delete [] data;
	
	/*texture ->BindTexture();
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
	
	return texture;
}

bool FontRenderer::ParseBounds(const std::string &strBounds, sm::Rect<int> &bounds)
{
	int x;
	int y;
	int width;
	int height;
	
	if (sscanf(strBounds.c_str(), "%u;%u;%u;%u", &x, &y, &width, &height) != 4)
		return false;
	
	bounds = sm::Rect<int>(x, y, width, height);
	return true;
}

FontRenderer* FontRenderer::LoadFromFile(const char *path, SpriteBatch *spriteBatch)
{
	assert(spriteBatch != NULL);

 	FontLetter texLetters[256];

	XMLNode *xmlDoc = XMLLoader::LoadFromFile(path);
	if (xmlDoc == NULL)
		return NULL;
	
	std::string bmpFilename = xmlDoc->GetAttribAsString("bitmap");
	if (bmpFilename.empty())
		return NULL;
	
	Path _path(path);
	Texture *tex = LoadFontBitmap(_path.GetPath() + bmpFilename);
	if (tex == NULL)
		return NULL;

	if (xmlDoc->GetName() != "alphabet")
	{
		delete xmlDoc;
		return NULL;
	}

	for (uint32_t i = 0; i < xmlDoc->GetChildrenCount(); i++)
	{
		std::string letter = (*xmlDoc)[i].GetAttribAsString("letter");
		std::string bounds = (*xmlDoc)[i].GetAttribAsString("bounds");

		sm::Rect<int> boundsValues;
		if (!ParseBounds(bounds, boundsValues))
			return NULL;

		texLetters[letter[0]].Size = sm::Point<int>(boundsValues.Width, boundsValues.Height);
		texLetters[letter[0]].Coords = TexPart(tex, boundsValues);
	}
	
	FontRenderer *fontRenderer = new FontRenderer();
	fontRenderer->m_spriteBatch = spriteBatch;
	if (fontRenderer != NULL)
	{
		memcpy(fontRenderer ->texLetters, texLetters, sizeof(FontLetter) * 256);
	}
	return fontRenderer;
}

FontRenderer::~FontRenderer()
{
}

void FontRenderer::DrawString(const char *text, unsigned x, unsigned y, const Color &color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_spriteBatch->Begin();
	unsigned xShift = x;
	unsigned yShift = y;
	unsigned rowHeight = texLetters['A'].Size.Y;
	for (unsigned i = 0; i < strlen(text); i++)
	{
		if (text[i] == '\n')
		{
			yShift += rowHeight;
			xShift = x;
			continue;
		}
		
		FontLetter letter = texLetters[text[i]];
		m_spriteBatch ->Draw(letter.Coords, color, xShift, yShift);//, letter.Size.X, letter.Size.Y);
		xShift += letter.Size.X;
		
	}
	//m_spriteBatch ->Draw(texLetters[text[0]].Coords.Tex, Color(255, 0, 0), 0, 40);
	m_spriteBatch->End();
}

sm::Point<int> FontRenderer::MeasureString(const char *text)
{
	sm::Point<int> size(0, texLetters['A'].Size.Y);
	
	for (uint32_t i = 0; i < strlen(text); i++)
		size.X += texLetters[text[i]].Size.X;
	
	return size;
}
