#pragma once

#include "ITransformable.h"
#include <Math\Matrix.h>

#include <vector>
#include <map>
#include <string>

class MeshPart;
class Property;
class Model;

class Mesh : public ITransformable
{
	friend class ModelLoader;

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
	sm::Matrix& BaseTransform();

	sm::Vec3 colorMask;

	Mesh *CreateReference();

	void Initialize();

public:
	std::vector<MeshPart*> meshParts;
	sm::Matrix transform;
	sm::Matrix animTransform;

	Model *model;

	bool m_isShadowCaster;
	bool m_isShadowReceiver;

	std::map<std::string, Property*> m_properties;
};
