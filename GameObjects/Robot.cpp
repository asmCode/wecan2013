#include "Robot.h"
#include "../DemoController.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <assert.h>

Robot::~Robot()
{
}

void Robot::Awake()
{
	Content *content = demo->m_content;

	m_robot = content->Get<Model>("robot");
	assert(m_robot != NULL);
	//m_robot->m_baseTransform = sm::Matrix::ScaleMatrix(0.01f, 0.01f, 0.01f);

	m_bindPose = content->Get<Animation>("bind_pose");
	assert(m_bindPose != NULL);

	m_testWalk = content->Get<Animation>("throw_wheel");
	assert(m_testWalk != NULL);

	m_allMeshParts.insert(m_allMeshParts.begin(), m_robot->m_meshParts.begin(), m_robot->m_meshParts.end());

	m_bindPose->AssignModel(m_robot);
	m_bindPose->ReplaceAnimation(m_testWalk);
}

void Robot::Update(float time, float seconds)
{
	m_bindPose->Update(time - 5.0f, sm::Matrix::IdentityMatrix(), seconds);
}

void Robot::Draw()
{
}

std::vector<MeshPart*>& Robot::GetMeshParts()
{
	return m_allMeshParts;
}

