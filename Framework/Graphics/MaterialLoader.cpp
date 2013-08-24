#include "MaterialLoader.h"
#include "Material.h"

#include <XML/XMLLoader.h>
#include <XML/XMLNode.h>
#include <IO/Path.h>

#include <assert.h>

Material* MaterialLoader::LoadFromFile(const std::string &path)
{
	XMLNode *rootNode = XMLLoader::LoadFromFile(path);
	if (rootNode == NULL)
		return NULL;

	Material *material = LoadMatarial(rootNode);

	delete rootNode;

	return material;
}

Material *MaterialLoader::LoadMatarial(XMLNode *rootNode)
{
	Material *mat = new Material();

	std::string elementName;
	mat->name = rootNode->GetAttribAsString("Name");

	for (uint32_t i = 0; i < rootNode->GetChildrenCount(); i++)
	{
		XMLNode &node = (*rootNode)[i];

		if (node.GetName() == "Param")
			LoadMaterialParam(mat, node);
		else if (node.GetName() == "Textures")
			LoadTextures(mat, node);
	}

	return mat;
}

bool MaterialLoader::LoadMaterialParam(Material *mat, XMLNode &node)
{
	std::string type = node.GetAttribAsString("Type");
	
	if (type == "Ambient")
	{
		mat->ambientColor = ParseVec4(node.GetAttribAsString("Value"));
	}
	else if (type == "Diffuse")
	{
		mat ->diffuseColor = ParseVec4(node.GetAttribAsString("Value"));
	}
	else if (type == "Specular")
	{
		mat ->specularColor = ParseVec4(node.GetAttribAsString("Value"));
	}
	else if (type == "Emissive")
	{
		mat ->emissiveColor = ParseVec4(node.GetAttribAsString("Value"));
	}
	else if (type == "Opacity")
	{
		mat ->opacity = node.GetAttribAsFloat("Value");
	}
	else if (type == "Glossiness")
	{
		mat ->glossiness = node.GetAttribAsFloat("Value");
	}
	else if (type == "SpecularLevel")
	{
		mat ->specularLevel = node.GetAttribAsFloat("Value");
	}
	else if (type == "EmissiveAmount")
	{
		mat ->emissiveAmount = node.GetAttribAsFloat("Value");
	}

	return true;
}

bool MaterialLoader::LoadTextures(Material *material, XMLNode &node)
{
	for (uint32_t i = 0; i < node.GetChildrenCount(); i++)
	{
		LoadTexture(material, node[i]);
	}

	return true;
}

bool MaterialLoader::LoadTexture(Material *material, XMLNode &node)
{
	std::string type = node.GetAttribAsString("Type");
	std::string file = node.GetAttribAsString("File");

	Path assetFilename(file);

	if (type == "Diffuse")
	{
		material ->diffuseTexName = assetFilename.GetFilename();
	}
	else if (type == "Opacity")
	{
		material ->opacityTexName = assetFilename.GetFilename();
	}
	else if (type == "Bump")
	{
		material ->normalTexName = assetFilename.GetFilename();
	}
	else if (type == "Lightmap")
	{
		material ->lightmapTexName = assetFilename.GetFilename();
	}
	else if (type == "Environment")
	{
		material->environmentTexName = assetFilename.GetFilename();
		material->reflectionValue = node.GetAttribAsFloat("Reflection");
	}

	return true;
}

sm::Vec4 MaterialLoader::ParseVec4(const std::string &value)
{
	sm::Vec4 color;
	sscanf(value.c_str(), "%f;%f;%f;%f", &color.x, &color.y, &color.z, &color.w);
	return color;
}

