#include "CreditsDance.h"
#include "../DemoController.h"
#include "../Billboard.h"
#include <Graphics/Content/Content.h>
#include <Utils/Log.h>
#include <Math/MathUtils.h>
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <assert.h>

CreditsDance::~CreditsDance()
{
}

void CreditsDance::Awake()
{
	m_baseTime = -1.0f;
	m_creditsIndex = -1;
	m_danceTime = 0.0f;
	m_danceTimeAddRatio = 1.0f;
	m_creditsProgress = 0.0f;
	m_creditsStartTime = 0.0f;
	m_creditsLength = 0.0f;
	m_creditsActive = false;

	Content *content = demo->m_content;

	m_creditsHandAnim = content->Get<Animation>("credits_dance_hand");
	assert(m_creditsHandAnim != NULL);

	m_handNodeAnim = m_creditsHandAnim->FindAnimationChild("RHand");
	assert(m_handNodeAnim != NULL);

	m_creditsHand = content->Get<Model>("credits_dance_hand");
	assert(m_creditsHand != NULL);

	m_creditsHand->m_meshParts[0]->m_alwaysHide = true;
	m_creditsHand->m_meshParts[1]->m_alwaysHide = true;
	m_creditsHand->m_meshParts[2]->m_alwaysHide = true;

	m_handTrg = m_creditsHand->FindMesh("credits_hand");
	assert(m_handTrg != NULL);

	m_handAxis = m_creditsHand->FindMesh("hand_axis");
	assert(m_handAxis != NULL);

	m_handUp = m_creditsHand->FindMesh("hand_up");
	assert(m_handUp != NULL);

	m_creditsVisible = m_handTrg->FindProperty("credits_visible");
	assert(m_creditsVisible != NULL);
	
	m_creditsHandAnim->AssignModel(m_creditsHand);

	m_creditSpriteShader = content->Get<Shader>("CreditSprite");
	assert(m_creditSpriteShader != NULL);

	uint32_t greetzCount = 12;
	m_greetzTexes = new Texture*[greetzCount];
	for (uint32_t i = 0; i < greetzCount; i++)
	{
		char texName[64];
		sprintf(texName, "greetz%d", i);
		m_greetzTexes[i] = content->Get<Texture>(texName);
		assert(m_greetzTexes[i] != NULL);
	}

	m_allMeshParts.insert(m_allMeshParts.end(), m_creditsHand->m_meshParts.begin(), m_creditsHand->m_meshParts.end());
}

void CreditsDance::Update(float time, float seconds)
{
	if (m_baseTime == -1.0f)
	{
		m_baseTime = 0.0f;
		m_danceTime += time;
	}

	m_danceTime += seconds * m_danceTimeAddRatio;

	m_creditsHandAnim->Update(m_danceTime, sm::Matrix::IdentityMatrix(), seconds);

	int keyframeIndex;

	if (m_creditsVisible->GetIntValue(time, &keyframeIndex) == 1)
	{
		m_d = true;

		if (!m_creditsActive)
		{
			m_creditsIndex++;
			m_creditsActive = true;

			assert(keyframeIndex < m_creditsVisible->GetKeysCount());
			
			int dummyInt;
			bool dummyBool;
			float startTime;
			float endTime;
			m_creditsVisible->GetKeyframe(keyframeIndex, startTime, dummyInt, dummyBool);
			m_creditsVisible->GetKeyframe(keyframeIndex + 1, endTime, dummyInt, dummyBool);

			//m_creditsHandAnim->Update(startTime, sm::Matrix::IdentityMatrix(), seconds);
			m_creditsHandAnim->SetAnimationTime(startTime, sm::Matrix::IdentityMatrix());
			sm::Matrix _axis = m_handAxis->Transform();
			sm::Matrix _up = m_handUp->Transform();
			sm::Matrix _trg = m_handTrg->Transform();
			sm::Vec3 startPos = m_handAxis->AnimTransform() * sm::Vec3(0, 0, 0);
			sm::Vec3 startDir = m_handTrg->AnimTransform() * sm::Vec3(0, 0, 0) - startPos;
			//startPos.Set(-100, 0, 0);
			//startDir.Set(0, 0, 1);
			startDir.Normalize();

			//m_creditsHandAnim->Update(endTime, sm::Matrix::IdentityMatrix(), seconds);
			m_creditsHandAnim->SetAnimationTime(endTime, sm::Matrix::IdentityMatrix());
			sm::Vec3 endPos = m_handAxis->AnimTransform() * sm::Vec3(0, 0, 0);
			sm::Vec3 endDir = m_handTrg->AnimTransform() * sm::Vec3(0, 0, 0) - startPos;
			//endPos.Set(100, 100, 100);
			//endDir.Set(0, 1, 0);
			endDir.Normalize();

			/*
			m_handAxis->Transform() = _axis;
			m_handUp->Transform() = _up;
			m_handTrg->Transform() = _trg;
			*/


			m_dir = (startDir + endDir).GetNormalized();
			m_dir = startDir;
			sm::Vec3 right = (endPos - startPos).GetNormalized();
			m_dir = sm::Vec3(0, 1, 0) * right;
			m_dir.Normalize();
			if (m_creditsIndex == 1 || m_creditsIndex == 4 || m_creditsIndex == 7 || m_creditsIndex == 10)
				m_dir.Reverse();
			m_up = right * m_dir;
			//m_up = m_dir * right;
			m_up.Normalize();

			m_creditsLength = endTime - startTime;
			m_creditsStartTime = startTime;

			m_startPos = startPos;

			m_creditTexWidth = (endPos - startPos).GetLength();
		}

		m_creditsProgress = (m_danceTime - m_creditsStartTime) / m_creditsLength;

		m_danceTimeAddRatio = MathUtils::Max(m_danceTimeAddRatio - seconds * 5.0f, 0.1f);
	}
	else
	{
		m_d = false;

		m_danceTimeAddRatio = MathUtils::Min(m_danceTimeAddRatio + seconds * 5.0f, 1.0f);
		m_creditsActive = false;
	}

	//demo->tmp_progress += seconds * 0.3f;
}

void CreditsDance::Draw()
{
}

void CreditsDance::DrawOpacities()
{
	Billboard::Setup();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);

	Texture *t = m_greetzTexes[m_creditsIndex];
	m_creditSpriteShader->UseProgram();
	m_creditSpriteShader->SetMatrixParameter(
		"u_mvp",
		demo->m_viewProj *
		sm::Matrix::TranslateMatrix(m_startPos) *
		sm::Matrix::CreateLookAt(m_dir, m_up) *
		sm::Matrix::ScaleMatrix(m_creditTexWidth, t->GetHeight() * (m_creditTexWidth / t->GetWidth()), 1) *
		sm::Matrix::TranslateMatrix(sm::Vec3(0, -0.5f, 0.0f)));
	m_creditSpriteShader->SetTextureParameter("u_tex", 0, t->GetId());
	m_creditSpriteShader->SetParameter("u_progress", m_creditsProgress);
	//m_creditSpriteShader->SetParameter("u_progress", 1.0f);

	Billboard::Draw();
	Billboard::Clean();
}

std::vector<MeshPart*>& CreditsDance::GetMeshParts()
{
	return m_allMeshParts;
}

float CreditsDance::GetAnimTime() const
{
	return m_danceTime;
}

bool CreditsDance::IsActive() const
{
	return m_creditsIndex != -1;
}

