#ifndef MATERIAL
#define MATERIAL

#include <Math\Vec4.h>
#include <string>

#include "Texture.h"
//#include "CubeTexture.h"

class Material
{
private:

public:
	std::string name;
	
	sm::Vec4 ambientColor;
	sm::Vec4 diffuseColor;
	sm::Vec4 specularColor;
	sm::Vec4 emissiveColor;

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
	//CubeTexture *environmentTex;

	std::string lightmapTexName;
	Texture *lightmapTex;

	Material(void);
	~Material(void);
};

#endif // MATERIAL

