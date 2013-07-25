#pragma once

#include <vector>
#include <map>
#include <string>
#include <Math\Matrix.h>

#include "MeshPart.h"
#include "ITransformable.h"

class Property;

class Mesh : public ITransformable
{
	friend class ModelLoader;

private:
	std::vector<MeshPart*> meshParts;
	sm::Matrix transform;
	sm::Matrix animTransform;

	std::map<std::string, Property*> m_properties;

public:
	int id;
	std::string name;
	sm::Matrix m_worldInverseMatrix;
	sm::Matrix m_worldMatrix;

	Mesh();
	~Mesh();

	void AddMeshPart(MeshPart *meshPart);
	std::vector<MeshPart*>& GetMeshParts();

	Property* FindProperty(const std::string &name);

	bool IsCam();
	int GetId();
	sm::Matrix& Transform();
	sm::Matrix& AnimTransform();

	sm::Vec3 colorMask;

	Mesh *CreateReference();
};
