#include "ModelLoader.h"

#include "VertexP.h"
#include "VertexPN.h"
#include "VertexPCN.h"
#include "VertexPC2N.h"
#include "Model.h"
#include "Mesh.h"
#include "MeshPart.h"
#include "VertexInformation.h"

// TODO: temporary include?
#include "Property.h"

#include <IO\BinaryReader.h>
#include <IO\Path.h>

#include <sstream>

unsigned short ModelLoader::m_fileVersion = 0;

Model* ModelLoader::LoadFromFile(const std::string &path)
{
	m_fileVersion = 0;

	uint8_t *buff;
	uint32_t fileSize;

	if (!Path::GetFileContent(path.c_str(), buff, fileSize))
	{
		assert(false && "couldn't load model file");
		return NULL;
	}

	Model *model = new Model();
	std::vector<Mesh*> &meshes = model ->GetMeshes();

	BinaryReader br(buff);

	if (fileSize >= 8)
	{
		BinaryReader *sigBr = new BinaryReader(buff);

		char fileSignature[6];
		sigBr->ReadBuffer(fileSignature, 6);
		unsigned short fileVersion = sigBr->Read<unsigned short>();

		if (memcmp("FTSMDL", fileSignature, 6) == 0)
		{
			m_fileVersion = fileVersion;

			// skip 8 bytes
			br.Read<int>();
			br.Read<int>();
		}
	}

	int meshesCount = br.Read<int>();

	for (int i = 0; i < meshesCount; i++)
	{
		Mesh *mesh = LoadMesh(br);
		mesh->Initialize();
		mesh->model = model;
		meshes.push_back(mesh);
	}

	delete [] buff;

	model->GetMeshParts(model->m_meshParts);

	return model;
}

Mesh* ModelLoader::LoadMesh(BinaryReader &br)
{
	Mesh *mesh = new Mesh();
	mesh ->id = br.Read<int>();
	mesh->name = br.Read<std::string>();

	if (m_fileVersion > 0)
	{
		for (int i = 0; i < 16; i++)
			mesh->m_worldInverseMatrix.a[i] = br.Read<float>();

		mesh->m_worldMatrix = mesh->m_worldInverseMatrix.GetInversed();
	}

	int meshPartsCount = br.Read<int>();

	for (int i = 0; i < meshPartsCount; i++)
	{
		MeshPart *meshPart = LoadMeshPart(br);
		meshPart->mesh = mesh;
		mesh ->AddMeshPart(meshPart);
	}

	int propsCount = br.Read<int>();
	for (int i = 0; i < propsCount; i++)
	{
		Property *prop = LoadProperty(br);
		mesh->m_properties[prop->GetName()] = prop;
	}

	return mesh;
}

MeshPart *ModelLoader::LoadMeshPart(BinaryReader &br)
{
	std::string materialName = br.Read<std::string>();
	uint8_t vertexType = br.Read<uint8_t>();
	int verticesCount = br.Read<int>();

	int vertexSize = VertexInformation::GetStride(vertexType);
	int vertexDataSize = vertexSize * verticesCount;
	
	uint8_t *vertices = new uint8_t[vertexDataSize];
	br.ReadBuffer((char*)vertices, vertexDataSize);

	MeshPart *meshPart = new MeshPart(verticesCount, vertices, NULL, vertexType);
	meshPart ->materialName = materialName;

	return meshPart;
}

Property* ModelLoader::LoadProperty(BinaryReader &br)
{
	std::string name = br.Read<std::string>();
	Property::PropertyType type = (Property::PropertyType)br.Read<BYTE>();
	Property::AnimationType animType = (Property::AnimationType)br.Read<BYTE>();

	Property *prop = new Property(name, type, animType);

	if (animType == Property::AnimationType_None)
	{
		switch (type)
		{
		case Property::PropertyType_Boolean:
			prop->SetValue(br.Read<bool>());
			break;

		case Property::PropertyType_Int:
			prop->SetValue(br.Read<int>());
			break;

		case Property::PropertyType_Float:
			prop->SetValue(br.Read<float>());
			break;

		case Property::PropertyType_Vector3:
			{
				sm::Vec3 val;
				val.x = br.Read<float>();
				val.y = br.Read<float>();
				val.z = br.Read<float>();
				prop->SetValue(val);
			}
			break;

		case Property::PropertyType_String:
			prop->SetValue(br.Read<std::string>());
			break;
		}
	}
	else
	{
		int keysCount = br.Read<int>();

		for (int i = 0; i < keysCount; i++)
		{
			float time = br.Read<float>();

			switch (type)
			{
			case Property::PropertyType_Float:
				{
					float value = br.Read<float>();
					prop->SetValue(value, time);
				}
				break;

			case Property::PropertyType_Int:
				{
					int value = br.Read<int>();
					prop->SetValue(value, time);
				}
				break;
			}
		}
	}

	return prop;
}

