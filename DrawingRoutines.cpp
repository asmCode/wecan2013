#include "DrawingRoutines.h"

#include "RobotElement.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Shader.h>
#include <Graphics/Material.h>
#include "GraphicsLibrary\DepthTexture.h"
#include "BasicLightingEffect.h"
#include <Graphics/MeshPart.h>
#include <Graphics/Mesh.h>
#include <Graphics/VertexType.h>
#include <Graphics/VertexInformation.h>
#include <assert.h>

sm::Matrix DrawingRoutines::m_viewProjMatrix;
sm::Vec3 DrawingRoutines::m_lightPosition;
sm::Vec3 DrawingRoutines::m_eyePosition;

Shader *DrawingRoutines::m_diffLightMapShader;
Shader *DrawingRoutines::m_diffNormLightmapShader;
Shader *DrawingRoutines::m_diffShader;
Shader *DrawingRoutines::m_colorShader;
Shader *DrawingRoutines::m_diffNormShader;
Shader *DrawingRoutines::m_blackShader;


//sm::Matrix DrawingRoutines::fixCoordsMatrix;
//
//DrawingRoutines::DrawingRoutines(void)
//{
//	fixCoordsMatrix = sm::Matrix::IdentityMatrix();
//	fixCoordsMatrix[0] = 0.5f;
//	fixCoordsMatrix[5] = 0.5f;
//	fixCoordsMatrix[10] = 0.5f;
//
//	fixCoordsMatrix[12] = 0.5f;
//	fixCoordsMatrix[13] = 0.5f;
//	fixCoordsMatrix[14] = 0.5f;
//}

bool DrawingRoutines::Initialize(Content *content)
{
	m_blackShader = content->Get<Shader>("Black");
	assert(m_blackShader != NULL);
	m_blackShader->BindVertexChannel(0, "a_position");
	m_blackShader->LinkProgram();

	m_colorShader = content->Get<Shader>("Color");
	assert(m_colorShader != NULL);
	m_colorShader->BindVertexChannel(0, "a_position");
	m_colorShader->BindVertexChannel(1, "a_normal");
	m_colorShader->LinkProgram();

	m_diffShader = content->Get<Shader>("Diff");
	assert(m_diffShader != NULL);
	m_diffShader->BindVertexChannel(0, "a_position");
	m_diffShader->BindVertexChannel(1, "a_coordsDiff");
	m_diffShader->BindVertexChannel(2, "a_normal");
	m_diffShader->LinkProgram();

	m_diffNormShader = content->Get<Shader>("DiffNorm");
	assert(m_diffNormShader != NULL);
	m_diffNormShader->BindVertexChannel(0, "a_position");
	m_diffNormShader->BindVertexChannel(1, "a_coordsDiff");
	m_diffNormShader->BindVertexChannel(2, "a_normal");
	m_diffNormShader->BindVertexChannel(3, "a_tangent");
	m_diffNormShader->LinkProgram();

	m_diffLightMapShader = content->Get<Shader>("DiffLightMap");
	assert(m_diffLightMapShader != NULL);
	m_diffLightMapShader->BindVertexChannel(0, "a_position");
	m_diffLightMapShader->BindVertexChannel(1, "a_coordsDiff");
	m_diffLightMapShader->BindVertexChannel(2, "a_coordsLightmap");
	m_diffLightMapShader->BindVertexChannel(3, "a_normal");
	m_diffLightMapShader->LinkProgram();

	m_diffNormLightmapShader = content->Get<Shader>("DiffNormLightmap");
	assert(m_diffNormLightmapShader != NULL);
	m_diffNormLightmapShader->BindVertexChannel(0, "a_position");
	m_diffNormLightmapShader->BindVertexChannel(1, "a_coordsDiff");
	m_diffNormLightmapShader->BindVertexChannel(2, "a_coordsLightmap");
	m_diffNormLightmapShader->BindVertexChannel(3, "a_normal");
	m_diffNormLightmapShader->BindVertexChannel(4, "a_tangent");
	m_diffNormLightmapShader->LinkProgram();

	return true;
}

void DrawingRoutines::SetViewProjMatrix(const sm::Matrix &viewProj)
{
	m_viewProjMatrix = viewProj;
}

void DrawingRoutines::SetLightPosition(const sm::Vec3 &lightPosition)
{
	m_lightPosition = lightPosition;
}

void DrawingRoutines::SetEyePosition(const sm::Vec3 &eyePosition)
{
	m_eyePosition = eyePosition;
}

bool DrawingRoutines::SetupShader(Material *material, MeshPart *meshPart, const sm::Matrix &worldatrix)
{
	if (!material->IsOpacity())
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(true);
		glDisable(GL_BLEND);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(false);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	assert(material != NULL);

	if (material->diffuseTex != NULL &&
		material->normalTex == NULL &&
		meshPart->m_lightmap == NULL &&
		material->opacityTex == NULL)
	{
		m_diffShader->UseProgram();
		m_diffShader->SetMatrixParameter("u_viewProjMatrix", m_viewProjMatrix);
		m_diffShader->SetParameter("u_lightPosition", m_lightPosition);
		m_diffShader->SetParameter("u_eyePosition", m_eyePosition);
		m_diffShader->SetMatrixParameter("u_worldMatrix", worldatrix);
		m_diffShader->SetTextureParameter("u_diffTex", 0, material->diffuseTex->GetId());
		m_diffShader->SetParameter("u_specularColor", material->specularColor);
		m_diffShader->SetParameter("u_opacity", material->Opacity());
		m_diffShader->SetParameter("u_glossiness", material->glossiness * 256.0f);
		m_diffShader->SetParameter("u_specularLevel", material->specularLevel);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		
		return true;
	}
	else if (material->diffuseTex != NULL &&
		 	 material->normalTex == NULL &&
	 	 	 meshPart->m_lightmap != NULL &&
			 material->opacityTex == NULL)
	{
		assert(VertexInformation::HasAttrib(meshPart->m_vertexType, VertexAttrib::Coords2));

		m_diffLightMapShader->UseProgram();
		m_diffLightMapShader->SetMatrixParameter("u_viewProjMatrix", m_viewProjMatrix);
		m_diffLightMapShader->SetParameter("u_lightPosition", m_lightPosition);
		m_diffLightMapShader->SetParameter("u_eyePosition", m_eyePosition);
		m_diffLightMapShader->SetMatrixParameter("u_worldMatrix", worldatrix);
		m_diffLightMapShader->SetTextureParameter("u_diffTex", 0, material->diffuseTex->GetId());
		m_diffLightMapShader->SetTextureParameter("u_lightmapTex", 1, meshPart->m_lightmap->GetId());
		m_diffLightMapShader->SetParameter("u_specularColor", material->specularColor);
		m_diffLightMapShader->SetParameter("u_opacity", material->Opacity());
		m_diffLightMapShader->SetParameter("u_glossiness", material->glossiness * 256.0f);
		m_diffLightMapShader->SetParameter("u_specularLevel", material->specularLevel);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glDisableVertexAttribArray(4);

		return true;
	}
	else if (material->diffuseTex != NULL &&
		 	 material->normalTex != NULL &&
	 	 	 meshPart->m_lightmap == NULL &&
			 material->opacityTex == NULL)
	{
		m_diffNormShader->UseProgram();
		m_diffNormShader->SetMatrixParameter("u_viewProjMatrix", m_viewProjMatrix);
		m_diffNormShader->SetParameter("u_lightPosition", m_lightPosition);
		m_diffNormShader->SetParameter("u_eyePosition", m_eyePosition);
		m_diffNormShader->SetMatrixParameter("u_worldMatrix", worldatrix);
		m_diffNormShader->SetTextureParameter("u_diffTex", 0, material->diffuseTex->GetId());
		m_diffNormShader->SetTextureParameter("u_normalTex", 1, material->normalTex->GetId());
		m_diffNormShader->SetParameter("u_specularColor", material->specularColor);
		m_diffNormShader->SetParameter("u_opacity", material->Opacity());
		m_diffNormShader->SetParameter("u_glossiness", material->glossiness * 256.0f);
		m_diffNormShader->SetParameter("u_specularLevel", material->specularLevel);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		
		return true;
	}
	else if (material->diffuseTex == NULL &&
		 	 material->normalTex == NULL &&
	 	 	 meshPart->m_lightmap == NULL &&
			 material->opacityTex == NULL)
	{
		m_colorShader->UseProgram();
		m_colorShader->SetMatrixParameter("u_viewProjMatrix", m_viewProjMatrix);
		m_colorShader->SetMatrixParameter("u_worldMatrix", worldatrix);
		m_colorShader->SetParameter("u_lightPosition", m_lightPosition);
		m_colorShader->SetParameter("u_eyePosition", m_eyePosition);
		m_colorShader->SetParameter("u_diffuseColor", material->diffuseColor);
		m_colorShader->SetParameter("u_specularColor", material->specularColor);
		m_colorShader->SetParameter("u_glossiness", material->glossiness * 256.0f);
		m_colorShader->SetParameter("u_specularLevel", material->specularLevel);
		
		glEnableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		
		return true;
	}
	else if (material->diffuseTex != NULL &&
		 	 material->normalTex != NULL &&
	 	 	 meshPart->m_lightmap != NULL &&
			 material->opacityTex == NULL)
	{
		assert(VertexInformation::HasAttrib(meshPart->m_vertexType, VertexAttrib::Coords2));

		m_diffNormLightmapShader->UseProgram();
		m_diffNormLightmapShader->SetMatrixParameter("u_viewProjMatrix", m_viewProjMatrix);
		m_diffNormLightmapShader->SetParameter("u_lightPosition", m_lightPosition);
		m_diffNormLightmapShader->SetParameter("u_eyePosition", m_eyePosition);
		m_diffNormLightmapShader->SetMatrixParameter("u_worldMatrix", worldatrix);
		m_diffNormLightmapShader->SetTextureParameter("u_diffTex", 0, material->diffuseTex->GetId());
		m_diffNormLightmapShader->SetTextureParameter("u_normalTex", 1, material->normalTex->GetId());
		m_diffNormLightmapShader->SetTextureParameter("u_lightmapTex", 2, meshPart->m_lightmap->GetId());
		m_diffNormLightmapShader->SetParameter("u_specularColor", material->specularColor);
		m_diffNormLightmapShader->SetParameter("u_opacity", material->Opacity());
		m_diffNormLightmapShader->SetParameter("u_glossiness", material->glossiness * 256.0f);
		m_diffNormLightmapShader->SetParameter("u_specularLevel", material->specularLevel);
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		
		return true;
	}
	else
	{
		int d = 0;
	}

	return false;
}

void DrawingRoutines::DrawWithMaterial(std::vector<MeshPart*> &meshParts)
{
	for (uint32_t i = 0; i < meshParts.size(); i++)
	{
		if (!meshParts[i]->IsVisible())
			continue;

		if (meshParts[i]->GetMaterial() == NULL)
		{
			//assert(false);
			meshParts[i]->material = new Material();
		}

		if (SetupShader(meshParts[i]->GetMaterial(), meshParts[i], meshParts[i]->mesh->Transform()))
			meshParts[i]->Draw();
	}
}

void DrawingRoutines::DrawBlack(std::vector<MeshPart*> &meshParts)
{
	m_blackShader->UseProgram();

	for (uint32_t i = 0; i < meshParts.size(); i++)
	{
		float opacity = 1.0f;
		if (meshParts[i]->GetMaterial() != NULL)
			opacity = meshParts[i]->GetMaterial()->Opacity();

		if (opacity == 1.0f)
		{
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			glDepthMask(true);
		}
		else
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(false);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			m_blackShader->SetParameter("u_opacity", opacity);
		}

		m_blackShader->SetMatrixParameter("u_mvp", m_viewProjMatrix * meshParts[i]->mesh->Transform());

		glEnableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);

		meshParts[i]->Draw();
	}
}

