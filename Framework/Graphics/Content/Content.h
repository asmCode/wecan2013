#ifndef CONTENT
#define CONTENT

#include <string>
#include <map>
#include <assert.h>

class IGraphicsEngine;
class Texture;
class Shader;
class Model;
class Animation;
class Material;

class Content
{
public:
	Content(IGraphicsEngine *graphicsEngine);
	~Content();

	void LoadTextures(const std::string &fullPath);
	void LoadShaders(const std::string &fullPath);
	void LoadModels(const std::string &fullPath);
	void LoadAnimations(const std::string &fullPath);
	void LoadMaterials(const std::string &fullPath);

	// Assign textures to materials and materials to models
	void CombineResources();

	template <typename T>
	void Add(const std::string &name, const T*resource)
	{
		std::map<std::string, T*> &resourcesMap = GetContentMap<T>();
		resourcesMap[name] = resource;
	}

	template <typename T>
	T* Get(const std::string &name)
	{
		std::map<std::string, T*> &resourcesMap = GetContentMap<T>();
		std::map<std::string, T*>::iterator it = resourcesMap.find(name);
		if (it == resourcesMap.end())
			return NULL;

		return it->second;
	}

private:
	IGraphicsEngine *m_graphicsEngine;

	std::map<std::string, Texture*> m_textures;
	std::map<std::string, Shader*> m_shaders;
	std::map<std::string, Model*> m_models;
	std::map<std::string, Animation*> m_animations;
	std::map<std::string, Material*> m_materials;

	template <typename T>
	std::map<std::string, T*>& GetContentMap()
	{
		assert(false);

		return m_textures; // just to hide warning
	}

	template <>
	std::map<std::string, Texture*>& GetContentMap<Texture>()
	{
		return m_textures;
	}

	template <>
	std::map<std::string, Shader*>& GetContentMap<Shader>()
	{
		return m_shaders;
	}

	template <>
	std::map<std::string, Model*>& GetContentMap<Model>()
	{
		return m_models;
	}

	template <>
	std::map<std::string, Animation*>& GetContentMap<Animation>()
	{
		return m_animations;
	}

	template <>
	std::map<std::string, Material*>& GetContentMap<Material>()
	{
		return m_materials;
	}
};

#endif // CONTENT


