#include "Material.h"

Material::Material()
{	
	ambientColor.Set(0.3f, 0.3f, 0.3f);
	diffuseColor.Set(0.6f, 0.6f, 0.6f);
	specularColor.Set(1.0f, 1.0f, 1.0f);
	emissiveColor.Set(0.0f, 0.0f, 0.0);

	opacity = 0.0f;
	glossiness = 0.3f;
	specularLevel = 1.0f;
	emissiveAmount = 0.0f;
	reflectionValue = 0.0f;

	diffuseTex = NULL;
	opacityTex = NULL;
	normalTex = NULL;
	//environmentTex = NULL;
}

Material::~Material(void)
{
}
