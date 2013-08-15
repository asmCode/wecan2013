#pragma once

#include <vector>

class Model;
class Mesh;
class MeshPart;

class GeometryBatch
{
public:
	GeometryBatch(void);
	~GeometryBatch(void);

	std::vector<Model*> m_models;

	std::vector<MeshPart*> allMeshParts;
	std::vector<MeshPart*> glowMeshParts;
	std::vector<MeshPart*> nonGlowMeshParts;
	std::vector<MeshPart*> opacityMeshParts;
	std::vector<MeshPart*> solidMeshParts;

	void AddModel(Model *model);

	void SetVisibility(bool visible);
	bool IsVisible() const;

private:
	bool m_isVisible;
};

