#pragma once

#include "Vertex.h"
#include "Material.h"

#include <string>
#include <stdint.h>

class Mesh;
class BoundingBox;
class BoundingSphere;

class MeshPart
{
public:
	uint32_t vboId;
	Material *material;

	int verticesCount;
	Vertex *vertices;
	Mesh *mesh;

	BoundingBox *bbox;
	BoundingSphere *bsphere;
	bool visible;
	bool isAlwaysVisible;

public:
	std::string materialName;

	MeshPart(int verticesCount, Vertex *vertices, Mesh *mesh);
	~MeshPart();

	void SetMaterial(Material *material);
	Material *GetMaterial();

	int GetVerticesCount();
	const Vertex* GetVertices();
	void SetVisibility(bool visible);
	bool IsVisible() const;
	bool& IsAlvaysVisible();

	// Set enable proper vertex channels before calling this method
	void Draw();
};
