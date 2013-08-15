#pragma once

#include <string>

class Model;
class Mesh;
class MeshPart;
class Property;
class BinaryReader;

class ModelLoader
{
public:
	static Model* LoadFromFile(const std::string &path);

private:
	static Mesh* LoadMesh(BinaryReader &br);
	static MeshPart *LoadMeshPart(BinaryReader &br);
	static Property *LoadProperty(BinaryReader &br);

	static unsigned short m_fileVersion;
};

