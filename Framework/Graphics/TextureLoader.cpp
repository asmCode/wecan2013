#include "TextureLoader.h"

#include "Texture.h"
#include <Graphics/ImageLoader.h>
#include <Graphics/ImageUtils.h>

Texture* TextureLoader::Load(const std::string &path)
{
	uint8_t *data;
	uint32_t width;
	uint32_t height;
	uint32_t bytesCount;

	if (!ImageLoader::LoadFromFile(path, data, width, height, bytesCount))
		return NULL;

	ImageUtils::FlipVertical(data, width, height, bytesCount);

	Texture *texture = new Texture(
		width,
		height,
		bytesCount * 8,
		data,
		Texture::Wrap_Repeat,
		Texture::Filter_LinearMipmapLinear,
		Texture::Filter_Linear,
		true);
	
	delete [] data;

	return texture;
}

