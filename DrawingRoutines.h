#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <vector>

class Shader;
class Model;
class MeshPart;
class Content;
class RobotElement;
class Material;
//class BasicLightingEffect;

class DrawingRoutines
{
private:
	static sm::Matrix m_viewProjMatrix;
	static sm::Vec3 m_lightPosition;
	static sm::Vec3 m_eyePosition;

	// diffuse lighting
	static Shader *m_diffLightShader;

	// diffuse lighting, diff tex, shadowmap tex
	static Shader *m_diffLightLightMapShader;

	// diffuse, normal, lightmap
	static Shader *m_diffNormLightmapShader;

	// diffuse texture, lighting
	static Shader *m_diffShader;

	// diffuse and specular lighting
	static Shader *m_colorShader;

	// diffuse texture, normal map
	static Shader *m_diffNormShader;

	// only black objects
	static Shader *m_blackShader;

	static bool SetupShader(Material *material, const sm::Matrix &worldatrix);
	
public:
	static bool Initialize(Content *content);

	static void SetViewProjMatrix(const sm::Matrix &viewProj);
	static void SetLightPosition(const sm::Vec3 &lightPosition);
	static void SetEyePosition(const sm::Vec3 &eyePosition);

	static void DrawWithMaterial(std::vector<MeshPart*> &meshParts);

	static void DrawDiffLight(Model *model);

	static void DrawBlack(std::vector<MeshPart*> &meshParts);

	static void DrawDiffLightLightMap(Model *model);

	static void DrawRobotElement(RobotElement *robotElement);

	/*void Initialize(
		Effect *stdLightingFx,
		Effect *stdLightingFx_GlowShadow,
		Effect *earlyZEffect);

	void DrawStandardLighting(
		std::vector<Model*> *models,
		const sm::Matrix &proj,
		const sm::Matrix &view,
		const sm::Matrix &world,
		const sm::Vec3 &lightPosition,
		const sm::Vec3 &eyePosition);

	void DrawStandardLighting(
		const std::vector<MeshPart*> &meshParts,
		const sm::Matrix &proj,
		const sm::Matrix &view,
		const sm::Matrix &world,
		const sm::Vec3 &lightPosition,
		const sm::Vec3 &eyePosition);

	void DrawStandardLighting_GlowShadow(
		const std::vector<MeshPart*> &meshParts,
		const sm::Matrix &proj,
		const sm::Matrix &view,
		const sm::Matrix &world,
		const sm::Vec3 &lightPosition,
		const sm::Vec3 &eyePosition,
		const sm::Matrix &lightWorld,
		const sm::Matrix &lightProj,
		unsigned int shadowMapTexId);

	void DrawGlowMask(
		const std::vector<MeshPart*> &meshParts,
		const sm::Matrix &proj,
		const sm::Matrix &view,
		const sm::Matrix &world);

	void DrawDepthBuffer(
		const std::vector<MeshPart*> &meshParts,
		const sm::Matrix &proj,
		const sm::Matrix &view,
		const sm::Matrix &world);

	static void DrawModel(Model *model);

	void DrawShadowMap(
		std::vector<MeshPart*> *meshParts,
		Effect *effect,
		const sm::Matrix &proj,
		const sm::Matrix &view,
		const sm::Matrix &world,
		bool lightPass);

	static void DrawCustomModel(
		Model *model,
		Effect *effect,
		const sm::Matrix &proj,
		const sm::Matrix &view,
		const sm::Matrix &world,
		const sm::Vec3 &eyePosition,
		const sm::Vec3 &lightPosition);

	static void DrawSpark(
		const sm::Matrix &proj,
		const sm::Matrix &view,
		const sm::Vec3 &start, 
		const sm::Vec3 &end,
		float power);*/
};
