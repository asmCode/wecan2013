//#include <assert.h>
//#include <Windows.h>
//#include <GL/glew.h>
//#include <GL/GL.h>
//#include "BasicLightingEffect.h"
//#include "GraphicsLibrary\Shader.h"
//#include "GraphicsLibrary\Material.h"
//
//BasicLightingEffect* GenericSingleton<BasicLightingEffect>::instance;
//
//BasicLightingEffect::BasicLightingEffect()
//{
//	stdLighting = NULL;
//	stdLighting_GlowShadow = NULL;
//}
//
//BasicLightingEffect::~BasicLightingEffect(void)
//{
//}
//
//void BasicLightingEffect::SetEffects(
//	Shader *stdLighting,
//	Shader *stdLighting_GlowShadow)
//{
//	this->stdLighting = stdLighting;
//	this->stdLighting_GlowShadow = stdLighting_GlowShadow;
//}
//
//void BasicLightingEffect::Begin()
//{
//	stdLighting->
//}
//
//void BasicLightingEffect::End()
//{
//	stdLighting ->EndPass(0);
//}
//
//void BasicLightingEffect::Begin2()
//{
//	stdLighting_GlowShadow ->BeginPass(0);
//}
//
//void BasicLightingEffect::End2()
//{
//	stdLighting_GlowShadow ->EndPass(0);
//}
//
//void BasicLightingEffect::SetupMaterial(const Material *material)
//{
//	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
//
//	if (material == NULL)
//	{
//		stdLighting ->SetCurrentTechnique("NoTexTech");
//		stdLighting ->SetParameter("ambientColor", 0.2f, 0.2f, 0.2f, 1.0f);
//		stdLighting ->SetParameter("diffuseColor", 0.6f, 0.6f, 0.6f, 1.0f);
//		stdLighting ->SetParameter("specularColor", 1.0f, 1.0f, 1.0f, 1.0f);
//		stdLighting ->SetParameter("opacity", 1.0f);
//		stdLighting ->SetParameter("glossiness", 64.0f);
//		stdLighting ->SetParameter("specularLevel", 1.0f);
//	}
//	else
//	{
//		stdLighting ->SetParameter("ambientColor", material ->ambientColor.x, material ->ambientColor.y, material ->ambientColor.z, 1.0f);
//		stdLighting ->SetParameter("diffuseColor", material ->diffuseColor.x, material ->diffuseColor.y, material ->diffuseColor.z, 1.0f);
//		stdLighting ->SetParameter("specularColor", material ->specularColor.x, material ->specularColor.y, material ->specularColor.z, 1.0f);
//		stdLighting ->SetParameter("emissiveColor", material ->emissiveColor.x, material ->emissiveColor.y, material ->emissiveColor.z, 1.0f);
//
//		//stdLighting ->SetParameter("glossiness", material ->glossiness * 100.0f);
//		stdLighting ->SetParameter("opacity", material->opacity);
//		stdLighting ->SetParameter("glossiness", 64.0f);
//		stdLighting ->SetParameter("specularLevel", material ->specularLevel * 0.4f);
//
//		if (material ->diffuseTex == NULL && material ->opacity < 1.0f)
//		{
//			stdLighting ->SetCurrentTechnique("BlendNoTexTech");
//		}
//		else if (material ->diffuseTex != NULL && material ->opacity < 1.0f)
//		{
//			stdLighting ->SetCurrentTechnique("BlendTexTech");
//			stdLighting ->SetTextureParameter("diffTex", material ->diffuseTex ->GetId());
//		}
//		else if (material ->diffuseTex == NULL && material ->normalTex == NULL)
//		{
//			stdLighting ->SetCurrentTechnique("NoTexTech");
//		}
//		else if (material ->diffuseTex != NULL && material ->normalTex == NULL)
//		{
//			stdLighting ->SetCurrentTechnique("DiffTexTech");
//			stdLighting ->SetTextureParameter("diffTex", material ->diffuseTex ->GetId());
//		}
//		else if (material ->diffuseTex == NULL && material ->normalTex != NULL)
//		{
//			stdLighting ->SetCurrentTechnique("BumpTech");
//			stdLighting ->SetTextureParameter("normTex", material ->normalTex ->GetId());
//		}
//		else if (material ->diffuseTex != NULL && material ->normalTex != NULL)
//		{
//			stdLighting ->SetCurrentTechnique("DiffTexBumpTech");
//			stdLighting ->SetTextureParameter("diffTex", material ->diffuseTex ->GetId());
//			stdLighting ->SetTextureParameter("normTex", material ->normalTex ->GetId());
//		}
//		else
//			assert(false);
//	}
//
//	glPopAttrib();
//}
//
//void BasicLightingEffect::SetupMaterial(
//	const Material *material,
//	unsigned int shadowMapTexId,
//	unsigned int glowTexId,
//	const sm::Matrix &shadowMapMatrix)
//{
//	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
//
//	stdLighting_GlowShadow->SetTextureParameter("shadowMap", shadowMapTexId);
//	stdLighting_GlowShadow->SetParameter("shadowMapMatrix", shadowMapMatrix);
//
//	if (material == NULL)
//	{
//		stdLighting_GlowShadow ->SetCurrentTechnique("NoTexTech_GlowShadow");
//		stdLighting_GlowShadow ->SetParameter("ambientColor", 0.2f, 0.2f, 0.2f, 1.0f);
//		stdLighting_GlowShadow ->SetParameter("diffuseColor", 0.6f, 0.6f, 0.6f, 1.0f);
//		stdLighting_GlowShadow ->SetParameter("specularColor", 1.0f, 1.0f, 1.0f, 1.0f);
//		stdLighting_GlowShadow ->SetParameter("opacity", 1.0f);
//		stdLighting_GlowShadow ->SetParameter("glossiness", 64.0f);
//		stdLighting_GlowShadow ->SetParameter("specularLevel", 1.0f);
//	}
//	else
//	{
//		stdLighting_GlowShadow ->SetParameter("ambientColor", material ->ambientColor.x, material ->ambientColor.y, material ->ambientColor.z, 1.0f);
//		stdLighting_GlowShadow ->SetParameter("diffuseColor", material ->diffuseColor.x, material ->diffuseColor.y, material ->diffuseColor.z, 1.0f);
//		stdLighting_GlowShadow ->SetParameter("specularColor", material ->specularColor.x, material ->specularColor.y, material ->specularColor.z, 1.0f);
//		stdLighting_GlowShadow ->SetParameter("emissiveColor", material ->emissiveColor.x, material ->emissiveColor.y, material ->emissiveColor.z, 1.0f);
//
//		//stdLighting_GlowShadow ->SetParameter("glossiness", material ->glossiness * 100.0f);
//		stdLighting_GlowShadow ->SetParameter("opacity", material->opacity);
//		stdLighting_GlowShadow ->SetParameter("glossiness", 64.0f);
//		stdLighting_GlowShadow ->SetParameter("specularLevel", material ->specularLevel * 0.4f);
//
//		if (material->diffuseTex != NULL &&
//			material->normalTex == NULL &&
//			material->opacityTex == NULL &&
//			material->environmentTex != NULL)
//		{
//			stdLighting_GlowShadow->SetCurrentTechnique("DiffEnvTech_GlowShadow");
//			//stdLighting_GlowShadow->SetCurrentTechnique("DiffTexTech_GlowShadow");
//			stdLighting_GlowShadow->SetTextureParameter("diffTex", material->diffuseTex->GetId());
//			stdLighting_GlowShadow->SetTextureParameter("envTex", material->environmentTex->GetTextureId());
//			stdLighting_GlowShadow->SetParameter("reflectionLevel", material->reflectionValue);
//		}
//		else if (material ->diffuseTex == NULL && material ->opacity < 1.0f)
//		{
//			stdLighting_GlowShadow ->SetCurrentTechnique("BlendNoTexTech_GlowShadow");
//		}
//		else if (material ->diffuseTex != NULL && material ->opacity < 1.0f)
//		{
//			stdLighting_GlowShadow ->SetCurrentTechnique("BlendTexTech_GlowShadow");
//			stdLighting_GlowShadow ->SetTextureParameter("diffTex", material ->diffuseTex ->GetId());
//		}
//		else if (material ->diffuseTex == NULL && material ->normalTex == NULL)
//		{
//			stdLighting_GlowShadow ->SetCurrentTechnique("NoTexTech_GlowShadow");
//		}
//		else if (material ->diffuseTex != NULL && material ->normalTex == NULL)
//		{
//			stdLighting_GlowShadow ->SetCurrentTechnique("DiffTexTech_GlowShadow");
//			stdLighting_GlowShadow ->SetTextureParameter("diffTex", material ->diffuseTex ->GetId());
//		}
//		else if (material ->diffuseTex == NULL && material ->normalTex != NULL)
//		{
//			stdLighting_GlowShadow ->SetCurrentTechnique("BumpTech_GlowShadow");
//			stdLighting_GlowShadow ->SetTextureParameter("normTex", material ->normalTex ->GetId());
//		}
//		else if (material ->diffuseTex != NULL && material ->normalTex != NULL)
//		{
//			stdLighting_GlowShadow ->SetCurrentTechnique("DiffTexBumpTech_GlowShadow");
//			stdLighting_GlowShadow ->SetTextureParameter("diffTex", material ->diffuseTex ->GetId());
//			stdLighting_GlowShadow ->SetTextureParameter("normTex", material ->normalTex ->GetId());
//		}
//		else
//			assert(false);
//
//		stdLighting_GlowShadow->SetTextureParameter("shadowMap", shadowMapTexId);
//		stdLighting_GlowShadow->SetParameter("shadowMapMatrix", shadowMapMatrix);
//	}
//
//	glPopAttrib();
//}
//
//void BasicLightingEffect::SetupParams(const sm::Matrix &proj,
//									  const sm::Matrix &view,
//									  const sm::Matrix &world,
//									  const sm::Vec3 &lightPos,
//									  const sm::Vec3 &eyePos,
//									  bool glowshadow)
//{
//	if (glowshadow)
//	{
//	stdLighting_GlowShadow ->SetParameter("mvp", proj * view * world);
//	stdLighting_GlowShadow ->SetParameter("world", world);
//	stdLighting_GlowShadow ->SetParameter("lightPosition", lightPos);
//	stdLighting_GlowShadow ->SetParameter("eyePosition", eyePos);
//	}
//	else
//	{
//	stdLighting ->SetParameter("mvp", proj * view * world);
//	stdLighting ->SetParameter("world", world);
//	stdLighting ->SetParameter("lightPosition", lightPos);
//	stdLighting ->SetParameter("eyePosition", eyePos);
//	}
//}
//
//
//void BasicLightingEffect::SetupEffectMaterial(
//	Shader *effect,
//	const Material *material)
//{
//	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
//
//	if (material == NULL)
//	{
//		effect ->SetCurrentTechnique("NoTexTech");
//		effect ->SetParameter("ambientColor", 0.2f, 0.2f, 0.2f, 1.0f);
//		effect ->SetParameter("diffuseColor", 0.6f, 0.6f, 0.6f, 1.0f);
//		effect ->SetParameter("specularColor", 1.0f, 1.0f, 1.0f, 1.0f);
//		effect ->SetParameter("opacity", 1.0f);
//		effect ->SetParameter("glossiness", 64.0f);
//		effect ->SetParameter("specularLevel", 1.0f);
//	}
//	else
//	{
//		effect ->SetParameter("ambientColor", material ->ambientColor.x, material ->ambientColor.y, material ->ambientColor.z, 1.0f);
//		effect ->SetParameter("diffuseColor", material ->diffuseColor.x, material ->diffuseColor.y, material ->diffuseColor.z, 1.0f);
//		effect ->SetParameter("specularColor", material ->specularColor.x, material ->specularColor.y, material ->specularColor.z, 1.0f);
//		effect ->SetParameter("emissiveColor", material ->emissiveColor.x, material ->emissiveColor.y, material ->emissiveColor.z, 1.0f);
//
//		//effect ->SetParameter("glossiness", material ->glossiness * 100.0f);
//		effect ->SetParameter("opacity", material->opacity);
//		effect ->SetParameter("glossiness", 64.0f);
//		effect ->SetParameter("specularLevel", material ->specularLevel * 0.4f);
//
//		if (material ->diffuseTex != NULL)
//		{
//			effect ->SetCurrentTechnique("DiffTexTech");
//			effect ->SetTextureParameter("diffTex", material->diffuseTex->GetId());
//		}
//		else if (material ->diffuseTex == NULL)
//		{
//			effect ->SetCurrentTechnique("NoTexTech");
//		}
//		else
//			assert(false);
//	}
//
//	glPopAttrib();
//}
//
//void BasicLightingEffect::SetupEffectParams(
//	Shader *effect,
//	const sm::Matrix &proj,
//	const sm::Matrix &view,
//	const sm::Matrix &world,
//	const sm::Vec3 &lightPos,
//	const sm::Vec3 &eyePos)
//{
//	effect ->SetParameter("mvp", proj * view * world);
//	effect ->SetParameter("world", world);
//	effect ->SetParameter("lightPosition", lightPos);
//	effect ->SetParameter("eyePosition", eyePos);
//}
