#include "Content.h"

#include "../IGraphicsEngine.h"
#include "../MeshPart.h"
#include "../Material.h"
#include "../Model.h"
#include <IO/Path.h>

Content::Content(IGraphicsEngine *graphicsEngine) :
	m_graphicsEngine(graphicsEngine)
{
}

Content::~Content()
{

}

void Content::LoadTextures(const std::string &fullPath)
{
	std::vector<std::string> filesNames;
	Path::GetAllFiles(filesNames, fullPath, "*.png");

	for (uint32_t i = 0 ; i < filesNames.size(); i++)
	{
		Path path(fullPath + filesNames[i]);

		m_textures[path.GetFilename()] = m_graphicsEngine->LoadTexture(path.GetFullPath());
	}
}

void Content::LoadShaders(const std::string &fullPath)
{
	std::vector<std::string> filesNames;
	Path::GetAllFiles(filesNames, fullPath, "*.vpr");

	for (uint32_t i = 0 ; i < filesNames.size(); i++)
	{
		// all vertex programs must have corresponding fragment program
		Path vprPath(fullPath + filesNames[i]);
		Path fprPath(fullPath + vprPath.GetFilename() + ".fpr");

		m_shaders[vprPath.GetFilename()] = m_graphicsEngine->LoadShader(vprPath.GetFullPath(), fprPath.GetFullPath());
	}
}

void Content::LoadModels(const std::string &fullPath)
{
	std::vector<std::string> filesNames;
	Path::GetAllFiles(filesNames, fullPath, "*.geo");

	for (uint32_t i = 0 ; i < filesNames.size(); i++)
	{
		Path path(fullPath + filesNames[i]);

		m_models[path.GetFilename()] = m_graphicsEngine->LoadModel(path.GetFullPath());
	}
}

void Content::LoadAnimations(const std::string &fullPath)
{
	std::vector<std::string> filesNames;
	Path::GetAllFiles(filesNames, fullPath, "*.ani");

	for (uint32_t i = 0 ; i < filesNames.size(); i++)
	{
		Path path(fullPath + filesNames[i]);

		m_animations[path.GetFilename()] = m_graphicsEngine->LoadAnimation(path.GetFullPath());
	}
}

void Content::LoadMaterials(const std::string &fullPath)
{
	std::vector<std::string> filesNames;
	Path::GetAllFiles(filesNames, fullPath, "*.mat");

	for (uint32_t i = 0 ; i < filesNames.size(); i++)
	{
		Path path(fullPath + filesNames[i]);

		m_materials[path.GetFilename()] = m_graphicsEngine->LoadMaterial(path.GetFullPath());
	}
}

void Content::CombineResources()
{
	// materials to models
	std::map<std::string, Model*>::iterator modelsIt;
	for (modelsIt = m_models.begin(); modelsIt != m_models.end(); ++modelsIt)
	{
		std::vector<MeshPart*> meshParts;
		modelsIt->second->GetMeshParts(meshParts);

		for (uint32_t i = 0; i < meshParts.size(); i++)
			meshParts[i]->SetMaterial(Get<Material>(meshParts[i]->materialName));
	}

	// textures to materials
	std::map<std::string, Material*>::iterator materialsIt;
	for (materialsIt = m_materials.begin(); materialsIt != m_materials.end(); ++materialsIt)
	{
		materialsIt->second->diffuseTex = Get<Texture>(materialsIt->second->diffuseTexName);
		materialsIt->second->normalTex = Get<Texture>(materialsIt->second->normalTexName);
		materialsIt->second->opacityTex = Get<Texture>(materialsIt->second->opacityTexName);
		// materialsIt->second->environmentTex = Get<CubeTexture>(materialsIt->second->environmentTexName); not implemented yet
	}
}

