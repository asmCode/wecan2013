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

Shader *DrawingRoutines::m_diffLightShader;
Shader *DrawingRoutines::m_diffLightLightMapShader;
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

	m_diffLightShader = content->Get<Shader>("DiffLight");
	assert(m_diffLightShader != NULL);
	m_diffLightShader->BindVertexChannel(0, "a_position");
	m_diffLightShader->BindVertexChannel(1, "a_coords");
	m_diffLightShader->BindVertexChannel(2, "a_normal");
	m_diffLightShader->LinkProgram();

	m_diffLightLightMapShader = content->Get<Shader>("DiffLightLightMap");
	assert(m_diffLightLightMapShader != NULL);
	m_diffLightLightMapShader->BindVertexChannel(0, "a_position");
	m_diffLightLightMapShader->BindVertexChannel(1, "a_coordsDiff");
	m_diffLightLightMapShader->BindVertexChannel(2, "a_coordsLightmap");
	m_diffLightLightMapShader->BindVertexChannel(3, "a_normal");
	m_diffLightLightMapShader->LinkProgram();

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

void DrawingRoutines::DrawDiffLight(Model *model)
{
	assert(model != NULL);

	std::vector<MeshPart*> meshParts;
	model->GetMeshParts(meshParts);

	glDepthMask(GL_TRUE);
	glEnableVertexAttribArray(0); 
	//glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	m_diffLightShader->UseProgram();
	m_diffLightShader->SetMatrixParameter("u_viewProjMatrix", m_viewProjMatrix);
	m_diffLightShader->SetParameter("u_lightPosition", m_lightPosition);

	for (uint32_t i = 0; i < meshParts.size(); i++)
	{
		m_diffLightShader->SetMatrixParameter("u_worldMatrix", meshParts[i]->mesh->Transform());

		meshParts[i]->Draw();
	}
	
	glDisableVertexAttribArray(0);
//	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void DrawingRoutines::DrawRobotElement(RobotElement *robotElement)
{
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	m_diffLightShader->UseProgram();
	m_diffLightShader->SetMatrixParameter("u_viewProjMatrix", m_viewProjMatrix);
	m_diffLightShader->SetMatrixParameter("u_worldMatrix", robotElement->m_animTransform * sm::Matrix::RotateAxisMatrix(-3.1415f / 2.0f, 1, 0, 0));
	//m_diffLightShader->SetMatrixParameter("u_worldMatrix", robotElement->m_animTransform);
	m_diffLightShader->SetParameter("u_lightPosition", m_lightPosition);

	for (uint32_t i = 0; i < robotElement->m_meshParts.size(); i++)
	{
		robotElement->m_meshParts[i]->Draw();
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(3);
}

void DrawingRoutines::DrawDiffLightLightMap(Model *model)
{
	assert(model != NULL);

	std::vector<MeshPart*> meshParts;
	model->GetMeshParts(meshParts);

	glDepthMask(GL_TRUE);
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	m_diffLightLightMapShader->UseProgram();
	m_diffLightLightMapShader->SetMatrixParameter("u_viewProjMatrix", m_viewProjMatrix);
	m_diffLightLightMapShader->SetParameter("u_lightPosition", m_lightPosition);

	for (uint32_t i = 0; i < meshParts.size(); i++)
	{
		m_diffLightLightMapShader->SetMatrixParameter("u_worldMatrix", meshParts[i]->mesh->Transform());

		if (meshParts[i]->GetMaterial() != NULL && meshParts[i]->GetMaterial()->diffuseTex != NULL)
			m_diffLightLightMapShader->SetTextureParameter("u_diffTex", 0, meshParts[i]->GetMaterial()->diffuseTex->GetId());
		if (meshParts[i]->GetMaterial() != NULL && meshParts[i]->GetMaterial()->lightmapTex != NULL)
			m_diffLightLightMapShader->SetTextureParameter("u_lightmapTex", 1, meshParts[i]->GetMaterial()->lightmapTex->GetId());

		//assert(meshParts[i]->m_vertexType == VertexType::PC2N);
		meshParts[i]->Draw();
	}
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

bool DrawingRoutines::SetupShader(Material *material, MeshPart *meshPart, const sm::Matrix &worldatrix)
{
	if (material->opacity == 1.0f)
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
		m_diffShader->SetMatrixParameter("u_worldMatrix", worldatrix);
		m_diffShader->SetTextureParameter("u_diffTex", 0, material->diffuseTex->GetId());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);

		return true;
	}
	else if (material->diffuseTex != NULL &&
		 	 material->normalTex == NULL &&
	 	 	 meshPart->m_lightmap != NULL &&
			 material->opacityTex == NULL)
	{
		assert(VertexInformation::HasAttrib(meshPart->m_vertexType, VertexAttrib::Coords2));

		m_diffLightLightMapShader->UseProgram();
		m_diffLightLightMapShader->SetMatrixParameter("u_viewProjMatrix", m_viewProjMatrix);
		m_diffLightLightMapShader->SetParameter("u_lightPosition", m_lightPosition);
		m_diffLightLightMapShader->SetMatrixParameter("u_worldMatrix", worldatrix);
		m_diffLightLightMapShader->SetTextureParameter("u_diffTex", 0, material->diffuseTex->GetId());
		m_diffLightLightMapShader->SetTextureParameter("u_lightmapTex", 1, meshPart->m_lightmap->GetId());

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

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		
		return true;
	}
	else if (material->diffuseTex == NULL &&
		 	 material->normalTex == NULL &&
	 	 	 meshPart->m_lightmap == NULL &&
			 material->opacityTex == NULL)
	{
		m_colorShader->UseProgram();
		m_colorShader->SetMatrixParameter("u_viewProjMatrix", m_viewProjMatrix);
		m_colorShader->SetParameter("u_lightPosition", m_lightPosition);
		m_colorShader->SetParameter("u_eyePosition", m_eyePosition);
		m_colorShader->SetParameter("u_diffuseColor", material->diffuseColor);
		m_colorShader->SetParameter("u_specularColor", material->specularColor);
		m_colorShader->SetParameter("u_opacity", material->opacity);
		m_colorShader->SetMatrixParameter("u_worldMatrix", worldatrix);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
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
			opacity = meshParts[i]->GetMaterial()->opacity;

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

