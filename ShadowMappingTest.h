//#pragma once
//
//#include "IScene.h"
//#include <Math\Matrix.h>
//#include "GraphicsLibrary\SteerCamera.h"
//#include "GraphicsLibrary\MeshPart.h"
//#include <vector>
//
//class Model;
//class Shader;
//class Texture;
//class DepthTexture;
//class Framebuffer;
//class DrawingRoutines;
//
//class ShadowMappingTest
//{
//private:
//	Effect *shadowMappingFx;
//	DepthTexture *depthTex;
//
//	Framebuffer *shadowMapFramebuffer;
//	Framebuffer *targetFramebuffer;
//
//	Texture *tex;
//	Texture *targetTex;
//	sm::Matrix fixCoordsMatrix;
//
//	int width;
//	int height;
//
//	float lightRot;
//
//	int lightMapWidth;
//	int lightMapHeight;
//	//sm::Matrix lightProjMatrix;
//
//	DrawingRoutines *drawingRoutines;
//
//public:
//	ShadowMappingTest(unsigned int width, unsigned int height);
//	~ShadowMappingTest(void);
//
//	//IScene Implementaion
//	void Initialize();
//	void Release();
//
//	void Update(float time, float ms);
//	void Draw(float time, float ms, const sm::Matrix &view, const sm::Matrix &proj);
//	void DrawLight(float time, float ms, const sm::Matrix &lightTransform, const sm::Matrix &proj);
//
//	unsigned int GetShadowTex();
//	unsigned int GetShadowMap();
//};
