#include "MechArm.h"

#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/Animation.h>
#include <Graphics/Content/Content.h>
#include "DemoController.h"
#include "GraphicsLibrary/Property.h"


MechArm::MechArm() :
	m_base(NULL),
	m_joint1(NULL),
	m_joint2(NULL),
	m_end(NULL),
	m_endAnim(NULL),
	m_sparksProp(NULL),
	m_endMesh(NULL),
	m_ctrlMesh(NULL),
	m_sparksGenerator(NULL)
{
	m_sparksGenerator = new SparksGenerator();
}

MechArm::~MechArm()
{
	if (m_sparksGenerator != NULL)
		delete m_sparksGenerator;
}

std::vector<Model*>& MechArm::GetModels()
{
	return m_models;
}

bool MechArm::Initialize(Content *content,
						 const std::string &basePath,
						 const std::string &mechPartsPrefix,
						 const std::string &sparksPropName,
						 const sm::Vec3 &basePos)
{
	Content *dc = content;

	m_basePos = basePos;

	// models
	m_base = dc->Get<Model>((mechPartsPrefix + "base").c_str());
	assert(m_base != NULL);
	m_joint1 = dc->Get<Model>((mechPartsPrefix + "joint1").c_str());
	assert(m_joint1 != NULL);
	m_joint2 = dc->Get<Model>((mechPartsPrefix + "joint2").c_str());
	assert(m_joint2 != NULL);
	m_end = dc->Get<Model>((mechPartsPrefix + "end").c_str());
	assert(m_end != NULL);

	// animations
	m_endAnim = dc->Get<Animation>((mechPartsPrefix + "end").c_str());
	assert(m_endAnim != NULL);

	m_endMesh = m_end->FindMesh("arm_end");
	assert(m_endMesh != NULL);
	m_ctrlMesh = m_end->FindMesh("_ctrl_props");
	assert(m_ctrlMesh != NULL);

	m_sparksProp = m_ctrlMesh->FindProperty(sparksPropName);
	assert(m_sparksProp != NULL);

	m_models.push_back(m_base);
	m_models.push_back(m_joint1);
	m_models.push_back(m_joint2);
	m_models.push_back(m_end);

	m_base->SetAlwaysVisible(true);
	m_joint1->SetAlwaysVisible(true);
	m_joint2->SetAlwaysVisible(true);
	m_end->SetAlwaysVisible(true);

	m_endAnim->AssignModel(m_end);

	return true;
}

void MechArm::Draw(float time, float seconds)
{
}

void MechArm::DrawSparks(float seconds, sm::Matrix &proj, sm::Matrix &view)
{
	m_sparksGenerator->Draw(seconds, proj, view);
}

void MechArm::Update(float time, float seconds)
{
	m_endAnim->Update(time, sm::Matrix::IdentityMatrix(), seconds);

	sm::Vec3 basePosition = m_basePos;
	sm::Vec3 endPosition = m_endMesh->AnimTransform() * sm::Vec3(0, 0, 0);
	float arm1Length = 450.0f;
	float arm2Length = 415.0f;

	sm::Matrix baseTransform;
	sm::Matrix arm1Transform;
	sm::Matrix arm2Transform;
	ArmResolver(basePosition, endPosition, arm1Length, arm2Length, baseTransform, arm1Transform, arm2Transform);

	m_base->SetTransformForMeshes(baseTransform);
	m_joint1->SetTransformForMeshes(arm1Transform);
	m_joint2->SetTransformForMeshes(arm2Transform);

	sm::Vec3 sparksSourcePosition = (m_endMesh->AnimTransform() * sm::Matrix::TranslateMatrix(0, 100.0f, 0)) * sm::Vec3(0, 0, 0);
	m_sparksGenerator->SetSourcePosition(sparksSourcePosition);

	if (m_sparksProp->GetIntValue(time) == 1)
		m_sparksGenerator->EnableSparksSource();
	else
		m_sparksGenerator->DisableSparksSource();

	m_sparksGenerator->Update(seconds);
}

sm::Matrix MechArm::CalcBoneMatrix(const sm::Vec3 &jointStart, const sm::Vec3 &jointEnd)
{
	sm::Matrix rot = sm::Matrix::IdentityMatrix();

	sm::Vec3 yAxis = (jointEnd - jointStart).GetNormalized();
	sm::Vec3 zAxis = (yAxis * sm::Vec3(1, 0, 0)).GetNormalized();
	sm::Vec3 xAxis = yAxis * zAxis;

	rot.a[0] = xAxis.x;
	rot.a[1] = xAxis.y;
	rot.a[2] = xAxis.z;

	rot.a[4] = yAxis.x;
	rot.a[5] = yAxis.y;
	rot.a[6] = yAxis.z;

	rot.a[8] = zAxis.x;
	rot.a[9] = zAxis.y;
	rot.a[10] = zAxis.z;

	return sm::Matrix::TranslateMatrix(jointStart) * rot;
}

sm::Matrix CalcBoneMatrixZ(const sm::Vec3 &jointStart, const sm::Vec3 &jointEnd)
{
	sm::Matrix rot = sm::Matrix::IdentityMatrix();

	sm::Vec3 zAxis = (jointEnd - jointStart).GetNormalized().GetReversed();
	sm::Vec3 xAxis = (zAxis * sm::Vec3(0, 1, 0)).GetNormalized();
	sm::Vec3 yAxis = xAxis * zAxis;

	rot.a[0] = xAxis.x;
	rot.a[1] = xAxis.y;
	rot.a[2] = xAxis.z;

	rot.a[4] = yAxis.x;
	rot.a[5] = yAxis.y;
	rot.a[6] = yAxis.z;

	rot.a[8] = zAxis.x;
	rot.a[9] = zAxis.y;
	rot.a[10] = zAxis.z;

	return sm::Matrix::TranslateMatrix(jointStart) * rot;
}

void MechArm::ArmResolver(const sm::Vec3 &basePosition,
					      const sm::Vec3 &targetPosition,
						  float arm1Length,
						  float arm2Length,
						  sm::Matrix &baseTransform,
						  sm::Matrix &arm1Transform,
						  sm::Matrix &arm2Transform)
{
	float targetDistance = (targetPosition - basePosition).GetLength();

	float d2 = ((arm2Length * arm2Length) - (arm1Length * arm1Length) + (targetDistance * targetDistance)) / (2 * targetDistance);
	float h = sqrt((arm2Length * arm2Length) - (d2 * d2));

	sm::Vec3 targetDirection = (targetPosition - basePosition).GetNormalized();
	sm::Vec3 axis = (targetDirection * sm::Vec3(0, 1, 0)).GetNormalized();
	sm::Vec3 hDirection = axis * targetDirection;

	sm::Vec3 arm1Position = basePosition + (targetDirection * (targetDistance - d2)) + hDirection * h;

	baseTransform = CalcBoneMatrixZ(basePosition, sm::Vec3(targetPosition.x, basePosition.y, targetPosition.z));
	arm1Transform = CalcBoneMatrixZ(basePosition, arm1Position);
	arm2Transform = CalcBoneMatrixZ(arm1Position, targetPosition);
}

