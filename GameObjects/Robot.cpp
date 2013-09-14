#include "Robot.h"
#include "../RobotElement.h"
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

	m_robotElementNamesMapping[Head] = "Head";
	m_robotElementNamesMapping[LFoot] = "LFoot";
	m_robotElementNamesMapping[RFoot] = "RFoot";
	m_robotElementNamesMapping[LShin] = "LShin";
	m_robotElementNamesMapping[RShin] = "RShin";
	m_robotElementNamesMapping[LThigh] = "LThigh";
	m_robotElementNamesMapping[RThigh] = "RThigh";

	m_robotElements = new RobotElement*[BodyIndicesCount];

	m_robot = content->Get<Model>("robot");
	assert(m_robot != NULL);

	m_bindPose = content->Get<Animation>("bind_pose");
	assert(m_bindPose != NULL);

	m_testWalk = content->Get<Animation>("throw_wheel");
	assert(m_testWalk != NULL);

	//for (uint32_t i = 0; i < BodyIndicesCount; i++)
	//{
	//	m_robotElements[i] = new RobotElement();
	//	m_robotElements[i]->Transform() = sm::Matrix::RotateAxisMatrix(3.1415f / 2.0f, 1, 0, 0);
	//	m_robotElements[i]->m_model = content->Get<Model>(m_robotElementNamesMapping[i]);
	//	assert(m_robotElements[i]->m_model != NULL);

	//	/*m_allMeshParts.insert(
	//		m_allMeshParts.begin(),
	//		m_robotElements[i]->m_model->m_meshParts.begin(),
	//		m_robotElements[i]->m_model->m_meshParts.end());

	//	for (uint32_t j = 0; j < m_robotElements[i]->m_model->m_meshParts.size(); j++)
	//		m_robotElements[i]->m_model->m_meshParts[j]->m_parentNode = m_robotElements[i];*/

	//	Animation *subAnim = m_testWalk->GetAnimationByNodeName(m_robotElementNamesMapping[i]);
	//	assert(subAnim != NULL);

	//	subAnim->mesh = m_robotElements[i];
	//}


	//m_testWalk->AssignModel(m_axis);
	m_allMeshParts.insert(m_allMeshParts.begin(), m_robot->m_meshParts.begin(), m_robot->m_meshParts.end());

	m_bindPose->AssignModel(m_robot);
	m_bindPose->ReplaceAnimation(m_testWalk);

	/*m_robot = content->Get<Model>("robot");
	assert(m_robot != NULL);

	m_robotAnim = content->Get<Animation>("robot");
	assert(m_robotAnim != NULL);

	m_robotAnim->AssignModel(m_robot);*/
}

void Robot::Update(float time, float seconds)
{
	m_bindPose->Update(time - 2.0f, sm::Matrix::IdentityMatrix(), seconds);
}

void Robot::Draw()
{
}

std::vector<MeshPart*>& Robot::GetMeshParts()
{
	return m_allMeshParts;
}

