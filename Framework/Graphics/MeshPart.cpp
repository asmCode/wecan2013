#include "MeshPart.h"

#include "VertexInformation.h"

#include "Mesh.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

#include <GL/glew.h>

MeshPart::MeshPart(int verticesCount, void *vertices, Mesh *mesh, uint8_t vertexType) :
	m_vertexType(vertexType)
{
	bbox = new BoundingBox();
	(*bbox) = BoundingBox::FromVertices(vertices, verticesCount, vertexType);
	bsphere = new BoundingSphere();
	(*bsphere) = BoundingSphere::FromBBox(*bbox);

	visible = true;
	isAlwaysVisible = false;

	this->mesh = mesh;
	this ->verticesCount = verticesCount;
	this ->vertices = vertices;
	
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * VertexInformation::GetStride(m_vertexType), vertices, GL_STATIC_DRAW);

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

	SetupVertexPointers();

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

const void* MeshPart::GetVertices()
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

void MeshPart::SetupVertexPointers()
{
	if (m_vertexType == VertexType::PCN)
	{
		uint32_t stride = VertexInformation::GetStride(m_vertexType);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(VertexInformation::GetOffset(m_vertexType, VertexAttrib::Position)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(VertexInformation::GetOffset(m_vertexType, VertexAttrib::Coords1)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(VertexInformation::GetOffset(m_vertexType, VertexAttrib::Normal)));
	}
}
