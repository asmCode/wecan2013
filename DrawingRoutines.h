#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <vector>

class Shader;
class Model;
class MeshPart;
class Content;
class RobotElement;
//class BasicLightingEffect;

class DrawingRoutines
{
private:
	//BasicLightingEffect *basicLightingEffect;
	//Effect *earlyZEffect;
	//static sm::Matrix fixCoordsMatrix;

	// diffuse lighting
	static Shader *m_diffLightShader;

public:
	static bool Initialize(Content *content);

	static void DrawDiffLight(
		Model *model,
		const sm::Matrix &viewProjMatrix,
		const sm::Vec3 &lightPosition);

	static void DrawRobotElement(
		RobotElement *robotElement,
		const sm::Matrix &viewProjMatrix,
		const sm::Vec3 &lightPosition);

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
