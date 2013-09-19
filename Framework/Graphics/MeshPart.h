#pragma once

#include "Material.h"

#include <string>
#include <stdint.h>

class Mesh;
class BoundingBox;
class BoundingSphere;
class ITransformable;

class MeshPart
{
public:
	uint32_t vboId;
	Material *material;
	Texture *m_lightmap;

	uint8_t m_vertexType;

	int verticesCount;
	void *vertices;
	Mesh *mesh;
	ITransformable *m_parentNode;

	BoundingBox *bbox;
	BoundingSphere *bsphere;
	bool visible;
	bool isAlwaysVisible;

	void SetupVertexPointers();

public:
	std::string materialName;

	MeshPart(int verticesCount, void *vertices, Mesh *mesh, uint8_t vertexType);
	~MeshPart();

	void SetMaterial(Material *material);
	Material *GetMaterial();

	int GetVerticesCount();
	const void* GetVertices();
	void SetVisibility(bool visible);
	bool IsVisible() const;
	bool& IsAlvaysVisible();

	// Set enable proper vertex channels before calling this method
	void Draw();

	bool m_alwaysHide;
};
