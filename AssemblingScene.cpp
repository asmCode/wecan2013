//#include "AssemblingScene.h"
//
//#include <Graphics/Model.h>
//#include "GraphicsLibrary\Texture.h"
//#include <Graphics/Content/Content.h>
//#include "Utils\Randomizer.h"
//#include "PropertySignal.h"
//#include "DemoController.h"
//#include "MechArm.h"
//#include "Robot.h"
//#include "GraphicsLibrary/Property.h"
//
//#include <Math\Matrix.h>
//#include <assert.h>
//
//const float AssemblingScene::StartCreditsTime = 10.0f;
//const float AssemblingScene::DancingStartTime = 180.0f + 20.7f;
//const float AssemblingScene::ReleasingTime = 60.0f * 1.0f + 50.96f;
//
//AssemblingScene::AssemblingScene(void) :
//	m_mechArm1(NULL),
//	m_mechArm2(NULL),
//	m_currentCreditsIndex(-1),
//	m_credits(NULL),
//	m_changeCredits(NULL),
//	m_switchedRobots(false),
//	m_robot(NULL),
//	m_screens(NULL),
//	m_screenIndex(0),
//	m_screenProgress(0.0f),
//	m_currentTime(0.0f)
//{
//	m_robotWorldMatrix =
//		sm::Matrix::TranslateMatrix(-6.859f, 380.929f, 1694.074f) *
//		sm::Matrix::RotateAxisMatrix(3.1415f, 0, 1, 0);
//		/*sm::Matrix::RotateAxisMatrix(3.1415f / 8.0f, 0, 0, 1) *
//		sm::Matrix::RotateAxisMatrix(3.1415f / 4.0f, 0, -1, 0);*/
//
//	m_dancingRobotWorldMatrix =
//		sm::Matrix::TranslateMatrix(6.289f, 365.769f, 158.514f) *
//		sm::Matrix::RotateAxisMatrix(3.1415f, 0, 1, 0);
//}
//
//AssemblingScene::~AssemblingScene(void)
//{
//	if (m_mechArm1 != NULL)
//		delete [] m_mechArm1;
//}
//
//bool AssemblingScene::Initialize()
//{
//	DemoContent *dc = DemoContent::GetInstance();
//
//	m_robot = demo->m_robot;
//	assert(m_robot != NULL);
//
//	m_mechArm1 = new MechArm();
//	m_mechArm1->Initialize("", "mech_arm_01_", "sparks", sm::Vec3(780.924f, 217.917f, 1819.266f));
//
//	m_mechArm2 = new MechArm();
//	m_mechArm2->Initialize("", "mech_arm_02_", "sparks2", sm::Vec3(-780.924f, 217.917f, 1819.266f));
//
//	m_credits = dc->Get<Model*>("credits");
//	assert(m_credits != NULL);
//
//	m_textures[0] = dc->Get<Texture*>("asmcode");
//	assert(m_textures[0] != NULL);
//	m_textures[1] = dc->Get<Texture*>("biter");
//	assert(m_textures[1] != NULL);
//	m_textures[2] = dc->Get<Texture*>("traymuss");
//	assert(m_textures[2] != NULL);
//
//	Property *changeCreditsProp = m_credits->GetMeshes()[0]->FindProperty("credits_signal");
//	assert(changeCreditsProp != NULL);
//	m_changeCredits = new PropertySignal(changeCreditsProp);
//
//	m_screens = dc->Get<Model*>("screens");
//	assert(m_screens != NULL);
//
//	char screenTexName[32];
//	for (uint32_t i = 0; i < HudFramesCount; i++)
//	{
//		sprintf(screenTexName, "hud%d", i);
//		m_hudScreensTex[i] = dc->Get<Texture*>(screenTexName);
//		assert(m_hudScreensTex[i] != NULL);
//	}
//
//	return true;
//}
//
//void AssemblingScene::Update(float time, float seconds)
//{
//	m_currentTime = time;
//
//	m_screenProgress += seconds * 25.0f;
//	m_screenIndex = (uint32_t)m_screenProgress % HudFramesCount;
//
//	m_mechArm1->Update(time, seconds);
//	m_mechArm2->Update(time, seconds);
//
//	if (time >= 180.0f + 20.598f)
//	{
//		m_robot->SetCurrentAction(Robot::Actions_Dancing);
//		m_robot->Update(time - (DancingStartTime + 1.2f), seconds);
//		m_robot->SetWorldMatrix(m_dancingRobotWorldMatrix);
//	}
//
//	if (time < ReleasingTime)
//	{
//		demo->m_geoBatch[DemoController::GeometryBatches_RobotMoving].SetVisibility(false);
//	}
//
//	if (! m_switchedRobots && time >= ReleasingTime)
//	{
//		m_switchedRobots = true;
//
//		demo->m_geoBatch[DemoController::GeometryBatches_RobotLying].SetVisibility(false);
//		demo->m_geoBatch[DemoController::GeometryBatches_RobotMoving].SetVisibility(true);
//
//		m_robot->SetCurrentAction(Robot::Actions_Releasing);
//	}
//
//	if ((time >= ReleasingTime) && (time < DancingStartTime))
//	{
//		m_robot->SetWorldMatrix(m_robotWorldMatrix);
//		m_robot->Update(fmodf((time - ReleasingTime) * 2.5f, m_robot->GetActionAnimLength(Robot::Actions_Releasing)), seconds);
//	}
//
//	if (time >= 115.0f && (time < DancingStartTime))
//	{
//		m_robot->SetWorldMatrix(demo->m_breakingWallScene->GetRobotPosition(time) * //sm::Matrix::RotateAxisMatrix(3.1415f, 0, 1, 0) *
//		sm::Matrix::RotateAxisMatrix(3.1415f / 8.0f, 0, 0, 1) *
//		sm::Matrix::RotateAxisMatrix(3.1415f / 4.0f, 0, -1, 0));
//		m_robot->SetCurrentAction(Robot::Actions_Running);
//		m_robot->Update(fmodf((time - 105.0f) * 2.0f, m_robot->GetActionAnimLength(Robot::Actions_Running)), seconds);
//	}
//
//	if (time >= (60.0f + 57.5f) && time <= (60.0f + 77.5f))
//	{
//		demo->m_geoBatch[DemoController::GeometryBatches_Passage].SetVisibility(true);
//	}
//
//	if (m_changeCredits->CheckSignal(time))
//	{
//		m_currentCreditsIndex++;
//		if (m_currentCreditsIndex >= 3)
//			m_currentCreditsIndex = 0;
//
//		creditsPower = 1.0f;
//	}
//	else
//	{
//		creditsPower -= seconds * (1.0f / 6.0f);
//		if (creditsPower < 0.0f)
//			creditsPower = 0.0f;
//	}
//}
//
//void AssemblingScene::Draw(float time, float seconds)
//{
//	m_mechArm1->DrawSparks(seconds, demo->proj, demo->view);
//	m_mechArm2->DrawSparks(seconds, demo->proj, demo->view);
//
//	DrawCredits(time, seconds);
//}
//
//void AssemblingScene::DrawGlows(float time, float seconds)
//{
//	m_mechArm1->DrawSparks(seconds, demo->proj, demo->view);
//	m_mechArm2->DrawSparks(seconds, demo->proj, demo->view);
//
//	DrawCredits(time, seconds);
//}
//
//void AssemblingScene::DrawOpacities(float time, float seconds)
//{
//	DrawScreen(time, seconds);
//}
//
//void AssemblingScene::SetVisibility(bool visible)
//{
//	if (visible)
//	{
//		demo->m_geoBatch[DemoController::GeometryBatches_MechArms].SetVisibility(true);
//		demo->m_geoBatch[DemoController::GeometryBatches_FactoryHall].SetVisibility(true);
//		demo->m_geoBatch[DemoController::GeometryBatches_RobotMoving].SetVisibility(true);
//	}
//	else
//	{
//		demo->m_geoBatch[DemoController::GeometryBatches_FactoryHall].SetVisibility(false);
//		demo->m_geoBatch[DemoController::GeometryBatches_MechArms].SetVisibility(false);
//		demo->m_geoBatch[DemoController::GeometryBatches_RobotLying].SetVisibility(false);
//	}
//}
//
//bool AssemblingScene::IsActiveOnTime(float time)
//{
//	return (time >= 0.0f && time <= 120.0f) ||
//		   (time >= 180.0f + 20.598f);
//}
//
//std::vector<Model*>& AssemblingScene::GetModels()
//{
//	return m_models;
//}
//
//void AssemblingScene::DrawCredits(float time, float seconds)
//{
//	if (m_currentCreditsIndex == -1)
//		return;
//
//	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glEnable(GL_TEXTURE_2D);
//	m_textures[m_currentCreditsIndex]->BindTexture();
//	//glDisable(GL_TEXTURE_2D);
//
//	glEnable(GL_BLEND);
//	//glDisable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glMultMatrixf(demo->proj);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glMultMatrixf(demo->view);
//
//	glColor4f(1, 1, 1, creditsPower);
//
//	m_credits->GetMeshes()[0]->GetMeshParts()[0]->DrawOnlyTex();
//
//	glPopAttrib();
//}
//
//void AssemblingScene::DrawScreen(float time, float seconds)
//{
//	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glEnable(GL_TEXTURE_2D);
//	m_hudScreensTex[m_screenIndex]->BindTexture();
//
//	glEnable(GL_BLEND);
//	glEnable(GL_DEPTH_TEST);
//	//glDisable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glMultMatrixf(demo->proj);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glMultMatrixf(demo->view);
//
//	glColor4f(1, 1, 1, 0.5f);
//
//	m_screens->GetMeshes()[0]->GetMeshParts()[0]->DrawOnlyTex();
//	m_screens->GetMeshes()[1]->GetMeshParts()[0]->DrawOnlyTex();
//
//	glPopAttrib();
//}
//
//void AssemblingScene::GetLightTransform(sm::Matrix &lightTransform)
//{
//	if (m_currentTime >= DancingStartTime)
//	{
//		lightTransform.a[0] = -0.8141f;
//		lightTransform.a[1] = 0.4034f;
//		lightTransform.a[2] = -0.4177f;
//		lightTransform.a[3] = 0.0000f;
//		lightTransform.a[4] = 0.0000f;
//		lightTransform.a[5] = 0.7193f;
//		lightTransform.a[6] = 0.6947f;
//		lightTransform.a[7] = 0.0000f;
//		lightTransform.a[8] = 0.5807f;
//		lightTransform.a[9] = 0.5655f;
//		lightTransform.a[10] = -0.5856f;
//		lightTransform.a[11] = 0.0000f;
//		lightTransform.a[12] = -419.9149f;
//		lightTransform.a[13] = -684.4364f;
//		lightTransform.a[14] = -1276.4071f;
//		lightTransform.a[15] = 1.0000f;
//	}
//}
//
