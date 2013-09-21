#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <vector>
#include <stdint.h>

class Shader;
class Model;
class MeshPart;
class Content;
class RobotElement;
class Material;

class DrawingRoutines
{
private:
	static sm::Matrix m_viewProjMatrix;
	static sm::Vec3 m_shadowLightPosition;
	static sm::Vec3 m_lightPosition;
	static sm::Vec3 m_eyePosition;

	static sm::Matrix m_lightViewMatrix;
	static sm::Matrix m_lightProjMatrix;

	// diffuse lighting, diff tex, shadowmap tex
	static Shader *m_diffLightMapShader;

	// diffuse, normal, lightmap
	static Shader *m_diffNormLightmapShader;

	// diffuse texture, lighting
	static Shader *m_diffShader;

	// diffuse and specular lighting
	static Shader *m_colorShader;

	// diffuse texture, normal map
	static Shader *m_diffNormShader;

	// only black objects with opacity
	static Shader *m_blackShader;

	// only black objects for shadowmap
	static Shader *m_shadowMapShader;

	// diffuse and specular lighting with shadow mapping (1 source)
	static Shader *m_sm_colorShader;

	// diffuse and specular lighting with shadow mapping (1 source)
	static Shader *m_sm_diffNormShader;

	// diffuse and specular lighting with shadow mapping (1 source)
	static Shader *m_sm_diffNormLightmapShader;

	static bool SetupShader(Material *material, MeshPart *meshPart, const sm::Matrix &worldatrix);
	static bool SetupShaderShadowMap(Material *material, MeshPart *meshPart, const sm::Matrix &worldatrix, uint32_t shadowMapId);
	
public:
	static bool Initialize(Content *content);

	static void SetViewProjMatrix(const sm::Matrix &viewProj);
	static void SetLightPosition(const sm::Vec3 &lightPosition);
	static void SetShadowLightPosition(const sm::Vec3 &shadowLightPosition);
	static void SetEyePosition(const sm::Vec3 &eyePosition);

	static void SetShadowCastingLightView(const sm::Matrix &lightViewMatrix);
	static void SetShadowCastingLightProj(const sm::Matrix &lightProjMatrix);

	static void DrawWithMaterial(std::vector<MeshPart*> &meshParts);
	static void DrawWithMaterialAndShadowMap(std::vector<MeshPart*> &meshParts, uint32_t shadowMapId);
	static void DrawBlack(std::vector<MeshPart*> &meshParts);
	static void DrawShadowMap(std::vector<MeshPart*> &meshParts);
};
