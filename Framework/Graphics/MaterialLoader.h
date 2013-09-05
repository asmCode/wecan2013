#ifndef MATERIAL_LOADER
#define MATERIAL_LOADER

#include <Math/Vec4.h>
#include <string>

class Material;
class XMLNode;

class MaterialLoader
{
public:
	static Material* LoadFromFile(const std::string &path);

private:
	static Material *LoadMatarial(XMLNode *rootNode);
	static bool LoadMaterialParam(Material *material, XMLNode &node);
	static bool LoadTextures(Material *material, XMLNode &node);
	static bool LoadTexture(Material *material, XMLNode &node);

	static sm::Vec3 ParseVec3(const std::string &value);
};

#endif // MATERIAL_LOADER

