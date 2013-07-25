#pragma once

#include "Texture.h"
#include "CubeTexture.h"

#include <string>
#include <Math\Vec3.h>

class Material
{
private:

public:
	std::string name;
	
	sm::Vec3 ambientColor;
	sm::Vec3 diffuseColor;
	sm::Vec3 specularColor;
	sm::Vec3 emissiveColor;

	float opacity;
	float glossiness;
	float specularLevel;
	float emissiveAmount;
	float reflectionValue; // for evironment mapping

	std::string diffuseTexName;
	Texture *diffuseTex;

	std::string opacityTexName;
	Texture *opacityTex;

	std::string normalTexName;
	Texture *normalTex;

	std::string environmentTexName;
	CubeTexture *environmentTex;

	Material(void);
	~Material(void);
};
