#include "Robot.h"

#include "RobotElement.h"
#include "DrawingRoutines.h"
#include "DemoController.h"
#include <Graphics/Model.h>
#include <Graphics/Animation.h>
#include <Graphics/Content/Content.h>
#include <IO\BinaryReader.h>
#include <fstream>

#include <assert.h>

static sm::Matrix CalcBoneMatrixZ(const sm::Vec3 &jointStart, const sm::Vec3 &jointEnd)
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

sm::Matrix CalcBoneMatrixNegY(const sm::Vec3 &jointStart, const sm::Vec3 &jointEnd, const sm::Vec3 &rightVector)
{
	sm::Matrix rot = sm::Matrix::IdentityMatrix();

	sm::Vec3 yAxis = (jointEnd - jointStart).GetNormalized().GetReversed();
	sm::Vec3 zAxis = (yAxis * rightVector).GetNormalized();
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

Robot::Robot(void)
{
}

Robot::~Robot(void)
{
}

bool Robot::Initialize(Content *content)
{
	//m_robotElementNamesMapping[Head] = "Head";
	//m_robotElementNamesMapping[LToe] = "LToe";
	//m_robotElementNamesMapping[RToe] = "RToe";
	//m_robotElementNamesMapping[LFoot] = "LFoot";
	//m_robotElementNamesMapping[RFoot] = "RFoot";
	//m_robotElementNamesMapping[LShin] = "LShin";
	//m_robotElementNamesMapping[RShin] = "RShin";

	//m_robotElements = new RobotElement*[BodyIndicesCount];

	//m_animation = content->Get<Animation>("animacja");
	//assert(m_animation != NULL);

	//for (uint32_t i = 0; i < BodyIndicesCount; i++)
	//{
	//	m_robotElements[i] = new RobotElement();
	//	m_robotElements[i]->m_model = content->Get<Model>(m_robotElementNamesMapping[i]);
	//	assert(m_robotElements[i]->m_model != NULL);

	//	m_robotElements[i]->m_model->GetMeshParts(m_robotElements[i]->m_meshParts);

	//	Animation *subAnim = m_animation->GetAnimationByNodeName(m_robotElementNamesMapping[i]);
	//	assert(subAnim != NULL);

	//	subAnim->mesh = m_robotElements[i];
	//}

	/*m_robot = content->Get<Model>("robot");
	assert(m_robot != NULL);

	m_robotAnim = content->Get<Animation>("robot");
	assert(m_robotAnim != NULL);

	m_robotAnim->AssignModel(m_robot);*/

	return true;
}

void Robot::SetCurrentAction(Robot::Actions action)
{
}

void Robot::SetWorldMatrix(const sm::Matrix &m)
{
}

void Robot::SetViewProjMatrix(const sm::Matrix &viewProj)
{
	m_viewProj = viewProj;
}

void Robot::Update(float time, float seconds)
{
	//m_animation->Update(time, sm::Matrix::IdentityMatrix(), seconds);

	m_robotAnim->Update(time, sm::Matrix::IdentityMatrix(), seconds);
}

void Robot::Draw(float time, float seconds)
{
	/*for (uint32_t i = 0; i < BodyIndicesCount; i++)
	{
		DrawingRoutines::DrawRobotElement(m_robotElements[i], m_viewProj, sm::Vec3(0, 0, 100));
	}*/

	DrawingRoutines::DrawDiffLight(m_robot);
}

std::vector<Model*>& Robot::GetModels()
{
	return std::vector<Model*>();
}

sm::Matrix Robot::CalcBoneMatrix(const sm::Vec3 &jointStart, const sm::Vec3 &jointEnd, const sm::Vec3 &rightVector)
{
	sm::Matrix rot = sm::Matrix::IdentityMatrix();

	sm::Vec3 yAxis = (jointEnd - jointStart).GetNormalized();
	sm::Vec3 zAxis = (yAxis * rightVector).GetNormalized();
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

float Robot::GetActionAnimLength(Robot::Actions action) const
{
	return 0.0f;
}

