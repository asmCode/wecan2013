#pragma once

#include "Vertex.h"
#include "Shader.h"
#include "Material.h"

#include "Utils.h"
#include <string>

class Mesh;
class BoundingBox;
class BoundingSphere;

class MeshPart
{
public:
	UINT vboId;
	Material *material;
	Shader *effect;

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

	void SetEffect(Shader *effect);
	Shader *GetEffect();

	void SetMaterial(Material *material);
	Material *GetMaterial();

	int GetVerticesCount();
	const Vertex* GetVertices();
	void SetVisibility(bool visible);
	bool IsVisible() const;
	bool& IsAlvaysVisible();

	void Draw();
	void DrawOnlyTex();
};
