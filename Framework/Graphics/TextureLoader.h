#pragma once

#include <string>

class Texture;

class TextureLoader
{
public:
	static Texture* Load(const std::string &path);
};

