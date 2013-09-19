#include "Robot.h"
#include "CreditsDance.h"
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

	m_robot = content->Get<Model>("bind_pose");
	assert(m_robot != NULL);
	//m_robot->m_baseTransform = sm::Matrix::ScaleMatrix(0.01f, 0.01f, 0.01f);

	m_bindPose = content->Get<Animation>("bind_pose");
	assert(m_bindPose != NULL);

	m_testWalk = content->Get<Animation>("throw_wheel");
	assert(m_testWalk != NULL);

	m_throwWheelAnim = content->Get<Animation>("throw_wheel");
	assert(m_throwWheelAnim != NULL);

	m_writtingAnim = content->Get<Animation>("writting");
	assert(m_writtingAnim != NULL);

	m_goToPostersAnim = content->Get<Animation>("go_to_posters");
	assert(m_goToPostersAnim != NULL);

	m_goToPostersModel = content->Get<Model>("go_to_posters");
	assert(m_goToPostersModel != NULL);

	m_creditsDance = content->Get<Animation>("credits_dance");
	assert(m_creditsDance != NULL);

	m_allMeshParts.insert(m_allMeshParts.begin(), m_robot->m_meshParts.begin(), m_robot->m_meshParts.end());
	m_allMeshParts.insert(m_allMeshParts.begin(), m_goToPostersModel->m_meshParts.begin(), m_goToPostersModel->m_meshParts.end());

	//float scaleFactor = (100.0f / 39.37f) * 2.8f;
	float scaleFactor = 1.0f;
	
	m_robot->m_baseTransform = sm::Matrix::ScaleMatrix(scaleFactor, scaleFactor, scaleFactor);

	m_bindPose->AssignModel(m_robot);
	m_bindPose->ReplaceAnimation(m_goToPostersAnim);


	m_goToPostersAnim->AssignModel(m_goToPostersModel);

	Animation *hip = m_bindPose->FindAnimationChild("Hip", true);

	Animation *wheelAnimation = m_goToPostersAnim->FindAnimationChild("futuris858", true);
	wheelAnimation->subAnims.push_back(hip);
	
	m_bindPose = m_goToPostersAnim;
}

void Robot::Update(float time, float seconds)
{
	//time = m_creditsDanceObject->GetAnimTime();
	m_bindPose ->Update(time, sm::Matrix::IdentityMatrix(), seconds);
}

void Robot::Draw()
{
}

std::vector<MeshPart*>& Robot::GetMeshParts()
{
	return m_allMeshParts;
}

void Robot::SetCreditsDance(CreditsDance *creditsDanceObject)
{
	m_creditsDanceObject = creditsDanceObject;
}

