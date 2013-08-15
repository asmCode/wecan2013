//#pragma once
//
//#include <Core/GenericSingleton.h>
//#include <Math\Vec3.h>
//#include <Math\Matrix.h>
//
//class Shader;
//class Material;
//
//class BasicLightingEffect : public GenericSingleton<BasicLightingEffect>
//{
//	friend class GenericSingleton<BasicLightingEffect>;
//
//public:
//	Shader *stdLighting;
//	Shader *stdLighting_GlowShadow;
//
//	BasicLightingEffect();
//	~BasicLightingEffect(void);
//
//public:
//	void SetEffects(
//		Shader *stdLighting,
//		Shader *stdLighting_GlowShadow);
//
//	void SetupMaterial(const Material *material);
//
//	void SetupMaterial(
//		const Material *material,
//		unsigned int shadowMapTexId,
//		unsigned int glowTexId,
//		const sm::Matrix &shadowMapMatrix);
//
//	void SetupParams(
//		const sm::Matrix &proj,
//		const sm::Matrix &view,
//		const sm::Matrix &world,
//		const sm::Vec3 &lightPos,
//		const sm::Vec3 &eyePos, bool glowshadow);
//	void Begin();
//	void End();
//	void Begin2();
//	void End2();
//
//	static void SetupEffectMaterial(Shader *effect, const Material *material);
//	static void SetupEffectParams(
//		Shader *effect,
//		const sm::Matrix &proj,
//		const sm::Matrix &view,
//		const sm::Matrix &world,
//		const sm::Vec3 &lightPos,
//		const sm::Vec3 &eyePos);
//};
