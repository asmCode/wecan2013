//#include "ShadowMappingTest.h"
//
//#include "GraphicsLibrary\Model.h"
//#include "GraphicsLibrary\DepthTexture.h"
//#include "GraphicsLibrary\Effect.h"
//#include "Content\DemoContent.h"
//#include "GraphicsLibrary\Framebuffer.h"
//#include "DrawingRoutines.h"
//#include "common.h"
//#include "DemoController.h"
//
//ShadowMappingTest::ShadowMappingTest(unsigned int width, unsigned int height)
//{
//	this->width = width;
//	this->height = height;
//
//	lightRot = 0.0f;
//
//	fixCoordsMatrix = sm::Matrix::IdentityMatrix();
//	fixCoordsMatrix[0] = 0.5f;
//	fixCoordsMatrix[5] = 0.5f;
//	fixCoordsMatrix[10] = 0.5f;
//
//	fixCoordsMatrix[12] = 0.5f;
//	fixCoordsMatrix[13] = 0.5f;
//	fixCoordsMatrix[14] = 0.5f;
//
//	lightMapWidth = width * 1;
//	lightMapHeight = height * 1;
//
//	/*lightMapWidth = 640;
//	lightMapHeight = 480;*/
//
//	//lightProjMatrix = sm::Matrix::PerspectiveMatrix(
//	//	60.0f, (float)lightMapWidth / (float)lightMapHeight, NEAR_PLANE, FAR_PLANE);
//}
//
//ShadowMappingTest::~ShadowMappingTest(void)
//{
//	delete shadowMapFramebuffer;
//	delete targetFramebuffer;
//	delete tex;
//	delete depthTex;
//	delete targetTex;
//}
//
//void ShadowMappingTest::Initialize()
//{
//	drawingRoutines = DrawingRoutines::GetInstance();
//	depthTex = new DepthTexture(lightMapWidth, lightMapHeight);
//	depthTex->BindTexture();
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB );
//	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL );
//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
//
//	tex = new Texture(lightMapWidth, lightMapHeight, 32, NULL, Texture::Wrap_ClampToEdge, Texture::Filter_Nearest, Texture::Filter_Nearest, false);
//	targetTex = new Texture(width, height, 32, NULL, Texture::Wrap_ClampToEdge, Texture::Filter_Nearest, Texture::Filter_Nearest, false);
//
//	shadowMapFramebuffer = new Framebuffer();
//	shadowMapFramebuffer ->Initialize(lightMapWidth, lightMapHeight, 32);
//	shadowMapFramebuffer ->BindFramebuffer();
//	shadowMapFramebuffer ->AttachDepthTexture(depthTex ->GetId());
//	shadowMapFramebuffer ->AttachColorTexture(tex ->GetId());
//	Framebuffer::RestoreDefaultFramebuffer();
//
//	targetFramebuffer = new Framebuffer();
//	targetFramebuffer ->Initialize(width, height, 32);
//	targetFramebuffer ->BindFramebuffer();
//	targetFramebuffer ->AttachColorTexture(targetTex ->GetId());
//	Framebuffer::RestoreDefaultFramebuffer();
//
//	shadowMappingFx = DemoContent::GetInstance() ->Get<Effect*>("shadow_mapping");
//}
//
//void ShadowMappingTest::Release()
//{
//}
//
//void ShadowMappingTest::Update(float time, float ms)
//{
//}
//
//void ShadowMappingTest::DrawLight(float time, float ms, const sm::Matrix &lightTransform, const sm::Matrix &proj)
//{
//	glPushAttrib(GL_VIEWPORT_BIT);
//
//	glViewport(0, 0, lightMapWidth, lightMapHeight);
//
//	sm::Matrix world = sm::Matrix::IdentityMatrix();
//
//	/*std::vector<Model*> models;
//	models.push_back(stage);*/
//
//	shadowMapFramebuffer ->BindFramebuffer();
//	shadowMapFramebuffer ->Validate();
//	glDepthMask(GL_TRUE);
//	glClear(GL_DEPTH_BUFFER_BIT);
//
//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset(1.0f, 0.01f);
//	drawingRoutines ->DrawShadowMap(&demo->solidMeshParts, shadowMappingFx, proj, lightTransform, world, true);
//	for (uint32_t i = 0; i < DemoController::GeometryBatches_Count; i++)
//	{
//		if (demo->m_geoBatch[i].IsVisible())
//			drawingRoutines ->DrawShadowMap(&demo->m_geoBatch[i].solidMeshParts, shadowMappingFx, proj, lightTransform, world, true);
//	}
//
//	glDisable(GL_POLYGON_OFFSET_FILL);
//
//	Framebuffer::RestoreDefaultFramebuffer();
//
//	glPopAttrib();
//}
//
//unsigned int ShadowMappingTest::GetShadowTex()
//{
//	return targetTex->GetId();
//}
//
//unsigned int ShadowMappingTest::GetShadowMap()
//{
//	return depthTex->GetId();
//}
