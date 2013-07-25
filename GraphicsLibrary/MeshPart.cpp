#include "MeshPart.h"
#include "Mesh.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

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

	/*sm::Vec3 *tangents = new sm::Vec3[verticesCount];
	Utils::CalcTangents(vertices, verticesCount / 3, tangents);

	for (int i = 0; i < verticesCount; i++)
		vertices[i].tangent = tangents[i];*/
	
	glGenBuffersARB(1, &vboId);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, verticesCount * sizeof(Vertex),
		vertices, GL_STATIC_DRAW_ARB);

	int positionOffset = 0;
	int normalOffset = positionOffset + sizeof(sm::Vec3);
	int tangentOffset = normalOffset + sizeof(sm::Vec3);
	int coordsOffset = tangentOffset + sizeof(sm::Vec3);

	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), (void*)positionOffset);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)normalOffset);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glTexCoordPointer(4, GL_FLOAT, sizeof(Vertex), (void*)tangentOffset);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), (void*)coordsOffset);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	material = NULL;
	effect = NULL;
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
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);

	int positionOffset = 0;
	int normalOffset = positionOffset + sizeof(sm::Vec3);
	int tangentOffset = normalOffset + sizeof(sm::Vec3);
	int coordsOffset = tangentOffset + sizeof(sm::Vec3);

	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), (void*)positionOffset);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)normalOffset);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glTexCoordPointer(4, GL_FLOAT, sizeof(Vertex), (void*)tangentOffset);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), (void*)coordsOffset);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLES, 0, verticesCount);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MeshPart::DrawOnlyTex()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);

	int positionOffset = 0;
	int normalOffset = positionOffset + sizeof(sm::Vec3);
	int tangentOffset = normalOffset + sizeof(sm::Vec3);
	int coordsOffset = tangentOffset + sizeof(sm::Vec3);

	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), (void*)positionOffset);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), (void*)coordsOffset);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLES, 0, verticesCount);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void MeshPart::SetEffect(Shader *effect)
{
	this ->effect = effect;
}

Shader *MeshPart::GetEffect()
{
	return effect;
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
