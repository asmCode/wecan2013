#pragma once

#include <Math/Matrix.h>
#include <vector>

class Mesh;
class MeshPart;

class Model
{
private:
	std::vector<Mesh*> meshes;

public:
	Model();
	~Model();

	std::vector<MeshPart*> m_meshParts;

	std::vector<Mesh*> &GetMeshes();
	void GetMeshParts(std::vector<MeshPart*> &meshParts);
	Mesh* FindMesh(const std::string &meshName);

	void SetTransformForMeshes(const sm::Matrix &transform);

	void SetAlwaysVisible(bool visible);

	Model *CreateReference();
};
