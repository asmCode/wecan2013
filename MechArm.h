#pragma once

#include <string>
#include <vector>
#include <Math\Matrix.h>

class Model;
class Mesh;
class Animation;
class Property;
class SparksGenerator;
class Content;

class MechArm
{
public:
	MechArm();
	~MechArm();

	bool Initialize(Content *content,
					const std::string &basePath,
					const std::string &mechPartsPrefix,
					const std::string &sparksPropName,
					const sm::Vec3 &basePos);

	void Draw(float time, float seconds);
	void Update(float time, float seconds);

	std::vector<Model*>& GetModels();

	void DrawSparks(float seconds, sm::Matrix &proj, sm::Matrix &view);

private:
	sm::Vec3 m_basePos;

	Model *m_base;
	Model *m_joint1;
	Model *m_joint2;
	Model *m_end;

	Mesh *m_endMesh;
	Mesh *m_ctrlMesh;
	Animation *m_endAnim;
	Property *m_sparksProp;

	SparksGenerator *m_sparksGenerator;

	std::vector<Model*> m_models;

	sm::Matrix CalcBoneMatrix(const sm::Vec3 &jointStart, const sm::Vec3 &jointEnd);

	void ArmResolver(const sm::Vec3 &basePosition,
					 const sm::Vec3 &targetPosition,
					 float arm1Length,
					 float arm2Length,
					 sm::Matrix &baseTransform,
					 sm::Matrix &arm1Transform,
					 sm::Matrix &arm2Transform);
};

