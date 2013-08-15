#include "MeshPart.h"

#include "Mesh.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

#include <GL/glew.h>

MeshPart::MeshPart(int verticesCount, Vertex *vertices, Mesh *mesh)
{
	bbox = new BoundingBox();
	(*bbox) = BoundingBox::FromVertices(vertices, verticesCount);
	bsphere = new BoundingSphere();
	(*bsphere) = BoundingSphere::FromBBox(*bbox);

	visible = true;
	isAlwaysVisible = false;

	this->mesh = mesh;
	this ->verticesCount = verticesCount;
	this ->vertices = vertices;
	
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(sm::Vec3)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(sm::Vec3) + sizeof(sm::Vec3)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	material = NULL;
}

bool& MeshPart::IsAlvaysVisible()
{
	return isAlwaysVisible;
}

MeshPart::~MeshPart()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glDeleteBuffersARB(1, &vboId);

	delete [] vertices;
	delete bbox;
	delete bsphere;
}

void MeshPart::Draw()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(sm::Vec3)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(sm::Vec3) + sizeof(sm::Vec3)));

	glDrawArrays(GL_TRIANGLES, 0, verticesCount);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void MeshPart::SetMaterial(Material *material)
{
	this ->material = material;
}

Material *MeshPart::GetMaterial()
{
	return material;
}

int MeshPart::GetVerticesCount()
{
	return verticesCount;
}

const Vertex* MeshPart::GetVertices()
{
	return vertices;
}

void MeshPart::SetVisibility(bool visible)
{
	this->visible = visible;
}

bool MeshPart::IsVisible() const
{
	return visible;
}

