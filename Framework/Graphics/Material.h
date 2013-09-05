#ifndef MATERIAL
#define MATERIAL

#include <Math\Vec3.h>
#include <Math\Vec4.h>
#include <string>

#include "Texture.h"
//#include "CubeTexture.h"

class Material
{
private:

public:
	std::string name;
	
	sm::Vec4 diffuseColor;
	sm::Vec3 specularColor;

	float glossiness;
	float specularLevel;
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

	float& Opacity();

	bool IsOpacity() const;
};

#endif // MATERIAL

