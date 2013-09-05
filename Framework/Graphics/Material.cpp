#include "Material.h"

Material::Material()
{	
	diffuseColor.Set(0.6f, 0.6f, 0.6f, 1.0f);
	specularColor.Set(1.0f, 1.0f, 1.0f);

	glossiness = 0.3f;
	specularLevel = 1.0f;
	reflectionValue = 0.0f;

	diffuseTex = NULL;
	opacityTex = NULL;
	normalTex = NULL;
	lightmapTex = NULL;
	//environmentTex = NULL;
}

Material::~Material(void)
{
}

float& Material::Opacity()
{
	return diffuseColor.w;
}

bool Material::IsOpacity() const
{
	return diffuseColor.w < 1.0f;
}