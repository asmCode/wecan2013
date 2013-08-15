#include "DrawingRoutines.h"

#include "RobotElement.h"
#include <Graphics/Content/Content.h>
#include <Graphics/Model.h>
#include <Graphics/Shader.h>
#include "GraphicsLibrary\DepthTexture.h"
#include "BasicLightingEffect.h"
#include <Graphics/MeshPart.h>
#include <Graphics/Mesh.h>

Shader *DrawingRoutines::m_diffLightShader;

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
//
//DrawingRoutines::~DrawingRoutines(void)
//{
//	BasicLightingEffect::DestroyInstance();
//}
//
//void DrawingRoutines::Initialize(
//	Effect *stdLightingFx,
//	Effect *stdLightingFx_GlowShadow,
//	Effect *earlyZEffect)
//{
//	basicLightingEffect = BasicLightingEffect::GetInstance();
//	basicLightingEffect ->SetEffects(stdLightingFx, stdLightingFx_GlowShadow);
//
//	this ->earlyZEffect = earlyZEffect;
//}
//
//void DrawingRoutines::DrawStandardLighting(std::vector<Model*> *models,
//										   const sm::Matrix &proj,
//										   const sm::Matrix &view,
//										   const sm::Matrix &world,
//										   const sm::Vec3 &lightPosition,
//										   const sm::Vec3 &eyePosition)
//{
//	assert(0);
//	for (int j = 0; j < (int)models ->size(); j++)
//	{
//		Model *model = (*models)[j];
//
//		std::vector<Mesh*> &meshes = model ->GetMeshes();
//
//		for (int i = 0; i < (int)meshes.size(); i++)
//		{
//			Mesh *mesh = meshes[i];
//
//			basicLightingEffect->stdLighting->SetParameter("colorMask", mesh->colorMask.x, mesh->colorMask.y, mesh->colorMask.z, mesh->colorMask.w);
//
//			const std::vector<MeshPart*> meshParts = mesh ->GetMeshParts();
//
//			for (int k = 0; k < (int)meshParts.size(); k++)
//			{
//				MeshPart *meshPart = meshParts[k];
//
//				if (!meshPart->IsVisible())
//					continue;
//
//				basicLightingEffect ->SetupMaterial(meshPart ->GetMaterial());
//				basicLightingEffect ->SetupParams(proj, view, meshPart->mesh->Transform(), lightPosition, eyePosition, false);
//
//				basicLightingEffect ->Begin();
//				meshPart ->Draw();
//				basicLightingEffect ->End();
//
//				////////////
//
//				//glPushMatrix();
//				//glLoadIdentity();
//				//glMultMatrixf((*view) * (*world));
//
//				//int verticesCount = meshPart ->GetVerticesCount();
//				//const Vertex *vertices = meshPart ->GetVertices();
//
//				//glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
//				//glDisable(GL_LIGHTING);
//				//glDisable(GL_TEXTURE_2D);
//				//glEnable(GL_DEPTH_TEST);
//
//				//glBegin(GL_LINES);
//
//				//glColor3f(1, 0, 0);
//				//for (int i = 0; i < verticesCount; i++)
//				//{
//				//	sm::Vec3 normPos = vertices[i].position + vertices[i].normal * 0.2f;
//
//				//	glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
//				//	glVertex3f(normPos.x, normPos.y, normPos.z);
//				//}
//
//				//glColor3f(0, 1, 0);
//				//for (int i = 0; i < verticesCount; i++)
//				//{
//				//	sm::Vec3 tanPos = vertices[i].position + vertices[i].tangent * 0.2f;
//
//				//	glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
//				//	glVertex3f(tanPos.x, tanPos.y, tanPos.z);
//				//}
//
//				///*glColor3f(1, 1, 1);
//				//for (int i = 0; i < verticesCount; i++)
//				//{
//				//	sm::Vec3 normPos = vertices[i].position + (vertices[i].tangent * vertices[i].normal).GetNormalized() * 0.2f;
//
//				//	glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
//				//	glVertex3f(normPos.x, normPos.y, normPos.z);
//				//}*/
//
//				//glPopMatrix();
//
//				//glEnd();
//
//				//glPopAttrib();
//				/////////////
//			}
//		}
//	}
//}
//
//void DrawingRoutines::DrawStandardLighting(const std::vector<MeshPart*> &meshParts,
//										   const sm::Matrix &proj,
//										   const sm::Matrix &view,
//										   const sm::Matrix &world,
//										   const sm::Vec3 &lightPosition,
//										   const sm::Vec3 &eyePosition)
//{
//	if (meshParts.size() > 0)
//	{
//		Mesh *mesh = meshParts[0]->mesh;
//		basicLightingEffect->stdLighting->SetParameter("colorMask", mesh->colorMask.x, mesh->colorMask.y, mesh->colorMask.z, mesh->colorMask.w);
//	}
//
//	for (int k = 0; k < (int)meshParts.size(); k++)
//	{
//		MeshPart *meshPart = meshParts[k];
//
//		if (!meshPart->IsVisible())
//			continue;
//
//		basicLightingEffect ->SetupMaterial(meshPart ->GetMaterial());
//		basicLightingEffect ->SetupParams(proj, view, meshPart->mesh->Transform(), lightPosition, eyePosition, false);
//
//		basicLightingEffect ->Begin();
//		meshPart ->Draw();
//		basicLightingEffect ->End();
//	}
//}
//
//void DrawingRoutines::DrawStandardLighting_GlowShadow(
//	const std::vector<MeshPart*> &meshParts,
//	const sm::Matrix &proj,
//	const sm::Matrix &view,
//	const sm::Matrix &world,
//	const sm::Vec3 &lightPosition,
//	const sm::Vec3 &eyePosition,
//	const sm::Matrix &lightWorld,
//	const sm::Matrix &lightProj,
//	unsigned int shadowMapTexId)
//{
//	sm::Matrix shadowMapMatrix = fixCoordsMatrix * lightProj * lightWorld;
//
//	for (int k = 0; k < (int)meshParts.size(); k++)
//	{
//		MeshPart *meshPart = meshParts[k];
//
//		if (!meshPart->IsVisible())
//			continue;
//
//		basicLightingEffect ->SetupMaterial(meshPart ->GetMaterial(), shadowMapTexId, 0, shadowMapMatrix);
//		basicLightingEffect ->SetupParams(proj, view, meshPart->mesh->Transform(), lightPosition, eyePosition, true);
//		//basicLightingEffect ->SetupParams(proj, view, sm::Matrix::IdentityMatrix(), lightPosition, eyePosition, true);
//
//		basicLightingEffect ->Begin2();
//		meshPart ->Draw();
//		basicLightingEffect ->End2();
//	}
//}
//
//void DrawingRoutines::DrawGlowMask(const std::vector<MeshPart*> &meshParts,
//								   const sm::Matrix &proj,
//								   const sm::Matrix &view,
//								   const sm::Matrix &world)
//{
//	Utils::PushProjModelMetrices();
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadMatrixf(proj);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadMatrixf(view * world);
//
//	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
//	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_LIGHTING);
//	glDisable(GL_BLEND);
//	glEnable(GL_DEPTH_TEST);
//	glColor3f(0, 0, 0);
//	glDepthMask(true);
//
//	for (int k = 0; k < (int)meshParts.size(); k++)
//	{
//		MeshPart *meshPart = meshParts[k];
//		if (!meshPart->IsVisible())
//			continue;
//
//		if (meshPart->material != NULL && meshPart->material->opacity < 1.0f)
//		{
//			glColor4f(0, 0, 0, meshPart->material->opacity);
//			glEnable(GL_BLEND);
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		}
//		else
//		{
//			glDisable(GL_BLEND);
//		}
//
//		glLoadMatrixf(view * meshPart->mesh->Transform());
//		meshPart ->Draw();
//	}
//
//	glPopAttrib();
//	Utils::PopProjModelMetrices();
//}
//
//void DrawingRoutines::DrawModel(Model *model)
//{
//	//assert(0);
//	std::vector<Mesh*> &meshes = model ->GetMeshes();
//
//	for (int i = 0; i < (int)meshes.size(); i++)
//	{
//		Mesh *mesh = meshes[i];
//
//		const std::vector<MeshPart*> &meshParts = mesh ->GetMeshParts();
//
//		for (int k = 0; k < (int)meshParts.size(); k++)
//		{
//			MeshPart *meshPart = meshParts[k];
//			if (!meshPart->IsVisible())
//				continue;
//
//			//meshPart ->Draw();
//			glBegin(GL_TRIANGLES);
//			for (int vi = 0; vi < meshPart ->verticesCount; vi++)
//			{
//				glTexCoord2f(meshPart ->vertices[vi].texCoord.x, meshPart ->vertices[vi].texCoord.y);
//				glVertex3f(meshPart ->vertices[vi].position.x, meshPart ->vertices[vi].position.y, meshPart ->vertices[vi].position.z);
//			}
//			glEnd();
//		}
//	}
//}
//
//void DrawingRoutines::DrawShadowMap(
//	std::vector<MeshPart*> *meshParts,
//	Effect *effect,
//	const sm::Matrix m_diffLightShader&proj,
//	const sm::Matrix &view,
//	const sm::Matrix &world,
//	bool lightPass)
//{
//	sm::Matrix projView = proj * view;
//
//	lightPass ? effect ->SetCurrentTechnique("ShadowMapping") : effect ->SetCurrentTechnique("ShadowMapping2");
//
//	for (int i = 0; i < (int)meshParts->size(); i++)
//	{
//		if (!(*meshParts)[i]->IsVisible())
//			continue;
//
//		effect ->SetParameter("mvp", projView * (*meshParts)[i]->mesh->Transform());
//
//		//effect ->SetParameter("mvp", projView/* * meshParts[i]->mesh->Transform()*/);
//		effect ->BeginPass(0);
//		(*meshParts)[i]->Draw();
//		effect ->EndPass(0);
//	}
//}
//
//void DrawingRoutines::DrawDepthBuffer(const std::vector<MeshPart*> &meshParts,
//									  const sm::Matrix &proj,
//									  const sm::Matrix &view,
//									  const sm::Matrix &world)
//{
//	assert(0);
//	earlyZEffect ->SetCurrentTechnique("EarlyZTech");
//
//	earlyZEffect ->BeginPass(0);
//
//	for (unsigned i = 0; i < meshParts.size(); i++)
//	{
//		if (!meshParts[i]->IsVisible())
//			continue;
//		earlyZEffect ->SetParameter("mvp", proj * view * meshParts[i]->mesh->Transform());
//		meshParts[i]->Draw();
//	}
//
//	earlyZEffect->EndPass(0);
//}
//
//void DrawingRoutines::DrawCustomModel(
//	Model *model,
//	Effect *effect,
//	const sm::Matrix &proj,
//	const sm::Matrix &view,
//	const sm::Matrix &world,
//	const sm::Vec3 &eyePosition,
//	const sm::Vec3 &lightPosition)
//{
//	std::vector<Mesh*> &meshes = model->GetMeshes();
//	for (unsigned i = 0; i < meshes.size(); i++)
//	{
//		std::vector<MeshPart*> &meshParts = meshes[i]->GetMeshParts();
//		for (unsigned j = 0; j < meshParts.size(); j++)
//		{
//			MeshPart *meshPart = meshParts[j];
//
//			if (!meshPart->IsVisible())
//				continue;
//
//			BasicLightingEffect::SetupEffectMaterial(effect, meshPart ->GetMaterial());
//			BasicLightingEffect::SetupEffectParams(effect, proj, view, meshPart->mesh->Transform(), lightPosition, eyePosition);
//
//			effect ->BeginPass(0);
//			meshPart ->Draw();
//			effect ->EndPass(0);
//		}
//	}
//}
//
//void DrawingRoutines::DrawSpark(const sm::Matrix &proj,
//								const sm::Matrix &view,
//								const sm::Vec3 &start, 
//							    const sm::Vec3 &end,
//								float power)
//{
//	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glLineWidth(2.0f);
//
//	glDisable(GL_TEXTURE_2D);
//	glEnable(GL_DEPTH_TEST);
//	glDepthMask(true);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glMultMatrixf(proj);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glMultMatrixf(view);
//
//	sm::Vec3 middle = (start + end) * 0.5f;
//
//	uint8_t r = 251;
//	uint8_t g = 190;
//	uint8_t b = 100;
//
//	glBegin(GL_LINE_STRIP);
//	glColor4ub(r, g, b, 0);
//	glVertex3f(start.x, start.y, start.z);
//	glColor4ub(r, g, b, (uint8_t)(150.0f * power));
//	glVertex3f(middle.x, middle.y, middle.z);
//	glColor4ub(r, g, b, 0);
//	glVertex3f(end.x, end.y, end.z);
//	glEnd();
//
//	glPopAttrib();
//}

bool DrawingRoutines::Initialize(Content *content)
{
	m_diffLightShader = content->Get<Shader>("DiffLight");
	assert(m_diffLightShader != NULL);

	m_diffLightShader->BindVertexChannel(0, "a_position");
	m_diffLightShader->BindVertexChannel(1, "a_normal");
	m_diffLightShader->LinkProgram();

	return true;
}

void DrawingRoutines::DrawDiffLight(Model *model, const sm::Matrix &viewProjMatrix, const sm::Vec3 &lightPosition)
{
	assert(model != NULL);

	std::vector<MeshPart*> meshParts;
	model->GetMeshParts(meshParts);

	glDepthMask(GL_TRUE);
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);

	m_diffLightShader->UseProgram();
	m_diffLightShader->SetMatrixParameter("u_viewProjMatrix", viewProjMatrix);
	m_diffLightShader->SetParameter("u_lightPosition", lightPosition);

	for (uint32_t i = 0; i < meshParts.size(); i++)
	{
		m_diffLightShader->SetMatrixParameter("u_worldMatrix", meshParts[i]->mesh->Transform());

		meshParts[i]->Draw();
	}
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void DrawingRoutines::DrawRobotElement(RobotElement *robotElement,
									   const sm::Matrix &viewProjMatrix,
									   const sm::Vec3 &lightPosition)
{
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);

	m_diffLightShader->UseProgram();
	m_diffLightShader->SetMatrixParameter("u_viewProjMatrix", viewProjMatrix);
	m_diffLightShader->SetMatrixParameter("u_worldMatrix", robotElement->m_animTransform * sm::Matrix::RotateAxisMatrix(-3.1415f / 2.0f, 1, 0, 0));
	//m_diffLightShader->SetMatrixParameter("u_worldMatrix", robotElement->m_animTransform);
	m_diffLightShader->SetParameter("u_lightPosition", lightPosition);

	for (uint32_t i = 0; i < robotElement->m_meshParts.size(); i++)
	{
		robotElement->m_meshParts[i]->Draw();
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

