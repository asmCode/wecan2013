#include "DemoController.h"
#include <Utils/Log.h>
#include "BasicLightingEffect.h"
#include "ShadowMappingTest.h"
#include "GraphicsLibrary/Framebuffer.h"
#include <Graphics/Model.h>
#include <Graphics/Mesh.h>
#include <Graphics/MeshPart.h>
#include "LoadingScreen.h"
#include <Graphics/Shader.h>
#include "Blur.h"
#include "ManCam.h"
#include "GraphicsLibrary\DepthTexture.h"
#include <Graphics\MeshPart.h>
#include "Frustum.h"
#include <Graphics/BoundingSphere.h>
#include "common.h"
#include "Robot.h"
#include "MechArm.h"
#include "AssemblingScene.h"
#include "GraphicsLibrary\Property.h"
#include "PropertySignal.h"
#include "DrawingRoutines.h"

#include <Graphics/TextureLoader.h>
#include <Graphics/ModelLoader.h>
#include <Graphics/MaterialLoader.h>
#include <Graphics/Shader.h>
#include <Graphics/AnimationLoader.h>
#include <Graphics/SpriteBatch.h>
#include <Graphics/FontRenderer.h>

//#define DISABLE_MUSIC 1
//#define DISABLE_FRUSTUM_CULLING 1
#define MAN_CAM 1
#define SHOW_FPS 1

DemoController* GenericSingleton<DemoController>::instance;
Randomizer DemoController::random;

DemoController::DemoController() :
	shadowPass(NULL),
	m_envTexture(NULL),
	m_robot(NULL),
	m_activeScene(NULL),
	demoEnded(false),
	m_doors(NULL),
	m_doorsAnim(NULL),
	m_fovSignal(NULL),
	m_fovPower(0.0f)
{
	fade = 0.0f;
	blurFbo = NULL;
	dofBlur = NULL;
	glowTex = NULL;
	framebuffer = NULL;
	isPlaying = false;
	demoMode = DemoMode_Demo;

	errorOccured = false;
	nextId = 0;

	glWnd = NULL;
	m_content = NULL;
	//scene = NULL;
	fov = 60.0f;

	isStereo = false;

	blur = NULL;

	currentCamera = NULL;
	cameraMode = CameraMode_Free;
	depthTex = NULL;

	electroNoiseVal = 0.0f;
	camShakeVal = 0.0f;

	Log::StartLog(false, true, false);
}

DemoController::~DemoController()
{
	Release();
}

void DemoController::InitializeProperties()
{
	if (demoMode == DemoMode_Freelook)
	{
		currentCamera = new ManCam();
	}
	else if (demoMode == DemoMode_Editor)
		currentCamera = new ManCam();
	else if (demoMode == DemoMode_Demo)
	{
		currentCamera = NULL;
		cameraMode = CameraMode_Preview;
	}
}

bool DemoController::Initialize(bool isStereo, DemoMode demoMode, HWND parent, const char *title, int width, int height,
								int bpp, int freq, bool fullscreen, bool createOwnWindow)
{
	eyeBlinkVal = 0.0f;
	eyeRangeVal = 0.0f;

	delay = 0.0f;
	delayLimit = 500.0f;
	fps = 0.0f;

	frustum = new Frustum();

	this ->demoMode = demoMode;
	this ->width = width;
	this ->height = height;
	this ->isStereo = isStereo;

	glWnd = new OpenglWindow();
	if (!glWnd ->Initialize(parent, title,
		width, height, bpp, freq, fullscreen, createOwnWindow, dynamic_cast<IKeyboardCallback*>(this)))
	{
		Log::LogT("couldn't initialize opengl window");
		Release();
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		assert(false);
	}

	SetOpenglParams();
	InitializeProperties();

	loadingScreen = new LoadingScreen();

	framebuffer = new Framebuffer();
	framebuffer ->Initialize(width, height, 32);

	targetTex0 = new Texture(width, height, 32, NULL, Texture::Wrap_ClampToEdge, Texture::Filter_Nearest, Texture::Filter_Nearest, false);

	glowTex = new Texture(width / 4, height / 4, 32, NULL, Texture::Wrap_ClampToEdge, Texture::Filter_Linear, Texture::Filter_Linear, false); 

	blurFbo = new Framebuffer();
	blurFbo ->Initialize(width / 4, height / 4 ,32);

	depthTex = new DepthTexture(demo ->width, demo ->height);
	depthTex->BindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
	framebuffer ->AttachDepthTexture(depthTex ->GetId());

	return true;
}

Animation *anim;

bool DemoController::LoadContent(const char *basePath)
{
	m_content = new Content(this);
	Content *dc = m_content;

	loadingScreen ->Initialize(basePath);

	m_strBasePath = basePath;

	//dc->AddContentObserver(this);
	dc->LoadModels(m_strBasePath + "models\\");
	dc->LoadModels(m_strBasePath + "models\\robot_elements\\");
	dc->LoadTextures(m_strBasePath + "textures\\");
	dc->LoadShaders(m_strBasePath + "effects\\");
	dc->LoadAnimations(m_strBasePath + "animations\\");
	dc->LoadMaterials(m_strBasePath + "materials\\");

	if (!AssignAssets())
		return false;

	m_spriteShader = m_content->Get<Shader>("Sprite");
	assert(m_spriteShader != NULL);
	m_spriteShader->BindVertexChannel(0, "a_position");
	m_spriteShader->BindVertexChannel(0, "a_coords");
	m_spriteShader->LinkProgram();

	m_spriteBatch = new SpriteBatch(m_spriteShader, sm::Matrix::Ortho2DMatrix(0, width, height, 0));
	m_fontRenderer = FontRenderer::LoadFromFile((m_strBasePath + std::string("fonts\\komika_title_32.xml")).c_str(), m_spriteBatch);

	anim = dc->Get<Animation>("animacja");
	Animation *headAnim = anim->GetAnimationByNodeName("Head");

	m_robot = new Robot();
	m_robot->Initialize(m_content);

	//demo ->activeCamera = demo ->manualCamera;

#ifndef DISABLE_MUSIC
	if (demoMode != DemoMode_Editor)
		music.LoadMusic((m_strBasePath + "\\music\\traymuss.mp3").c_str());
#endif

	//m_mdl_factory = dc->Get<Model*>("factory");
	//assert(m_mdl_factory != NULL);

	//m_robot = new Robot();
	//m_robot->Initialize(m_strBasePath, m_content);
	//m_robot->SetCurrentAction(Robot::Actions_BreakingWall);
	//m_robot->Update(1000.0f, 10.0f);

	//Model *factoryHal = dc->Get<Model>("factory_hal");
	//assert(factoryHal != NULL);
	//m_geoBatch[GeometryBatches_FactoryHall].AddModel(factoryHal);

	//Model *factoryPassage = dc->Get<Model>("factory_passage");
	//assert(factoryPassage != NULL);
	//m_geoBatch[GeometryBatches_Passage].AddModel(factoryPassage);
	//m_geoBatch[GeometryBatches_Passage].AddModel(dc->Get<Model>("robot_path"));

	//m_geoBatch[GeometryBatches_MechArms].AddModel(dc->Get<Model>("mech_arm_01_base"));
	//m_geoBatch[GeometryBatches_MechArms].AddModel(dc->Get<Model>("mech_arm_01_joint1"));
	//m_geoBatch[GeometryBatches_MechArms].AddModel(dc->Get<Model>("mech_arm_01_joint2"));
	//m_geoBatch[GeometryBatches_MechArms].AddModel(dc->Get<Model>("mech_arm_01_end"));
	//m_geoBatch[GeometryBatches_MechArms].AddModel(dc->Get<Model>("mech_arm_02_base"));
	//m_geoBatch[GeometryBatches_MechArms].AddModel(dc->Get<Model>("mech_arm_02_joint1"));
	//m_geoBatch[GeometryBatches_MechArms].AddModel(dc->Get<Model>("mech_arm_02_joint2"));
	//m_geoBatch[GeometryBatches_MechArms].AddModel(dc->Get<Model>("mech_arm_02_end"));

	//Model *robotLying = dc->Get<Model>("robot_lying");
	//assert(robotLying != NULL);
	//m_geoBatch[GeometryBatches_RobotLying].AddModel(robotLying);

	//std::vector<Model*> &robotModels = m_robot->GetModels();
	//for (uint32_t i = 0; i < robotModels.size(); i++)
	//{
	//	m_geoBatch[GeometryBatches_RobotMoving].AddModel(robotModels[i]);
	//	robotModels[i]->SetAlwaysVisible(true);
	//}
	//m_geoBatch[GeometryBatches_RobotMoving].SetVisibility(false);

	///*m_assemblingScene = new AssemblingScene();
	//m_assemblingScene->Initialize();*/

	////m_activeScene = m_assemblingScene;
	////m_activeScene->SetVisibility(true);

	////m_mdl_korytarz = dc->Get<Model*>("korytarz");
	////assert(m_mdl_korytarz != NULL);

	//m_envTexture = new CubeTexture(m_strBasePath + "\\images\\env_");

	////m_mdl_teapot = dc->Get<Model*>("teapot");
	////assert(m_mdl_teapot != NULL);

	camerasAnimation = dc->Get<Animation>("cameras");
	assert(camerasAnimation != NULL);
	animCamsMng.Load(m_strBasePath + "cameras\\cameras.cam", camerasAnimation);

	//if (width <= 1024)
	//	mask = dc->Get<Texture>("mask");
	//else
	//	mask = dc->Get<Texture>("mask_big");

	//assert(mask != NULL);

	//mask->BindTexture();
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//
	//InitElectroNoise();

	return true;
}

void DemoController::Release()
{
#ifndef DISABLE_MUSIC
	if (demoMode != DemoMode_Editor)
		music.Stop();
#endif

	if (m_content != NULL)
	{
		delete m_content;
		m_content = NULL;
	}

	if (framebuffer != NULL)
	{
		delete framebuffer;
		framebuffer = NULL;
	}

	if (glWnd != NULL)
	{
		glWnd ->Release();
		delete glWnd;
		glWnd = NULL;
	}

	if (shadowPass != NULL)
	{
		delete shadowPass;
		shadowPass = NULL;
	}

	if (blur != NULL)
	{
		delete blur;
		blur = NULL;
	}

	if (dofBlur != NULL)
	{
		delete dofBlur;
		dofBlur = NULL;
	}

	DeleteObject(depthTex);
	DeleteObject(glowTex);
	DeleteObject(blurFbo);
	DeleteObject(m_envTexture);
}

float lightRot = 0.0f;

bool started = false;

static float lastTime;
bool DemoController::Update(float time, float ms)
{
	time /= 1000.0f;
	float seconds = ms / 1000.0f;

	ICamera *cam = NULL;

#if MAN_CAM
	manCam.Process(ms);
	cam = &manCam;
#else
	camerasAnimation->Update(time, sm::Matrix::IdentityMatrix(), seconds);
	cam = animCamsMng.GetActiveCamera(time);
#endif

	

	m_viewProj =
		sm::Matrix::PerspectiveMatrix((cam->GetFov(time) / 3.1415f) * 180.0f, (float)width / (float)height, 0.1f, 200.0f) *
		cam->GetViewMatrix();

	//anim->Update(time / 1000.0f, sm::Matrix::IdentityMatrix(), seconds);

	m_robot->SetViewProjMatrix(m_viewProj);
	m_robot->Update(time, seconds);

//	if (demoMode != DemoMode_Editor && !started)
//	{
//#ifndef DISABLE_MUSIC
//		music.SetPosition(time / 1000.0f);
//		music.Play();
//
//		//music.SetPosition(time / 1000.0f);
//#endif
//		started = true;
//	}
//
//	/*
//	if (!demoEnded)
//	{
//		time = music.GetPosition() * 1000.0f;
//
//		if ((time / 1000.0f) >= (180.0f + 43.0f))
//		{
//			music.Stop();
//			demoEnded = true;
//		}
//	}
//	*/
//
//	lastTime = time;
//
//	m_geoBatch[GeometryBatches_FactoryHall].SetVisibility(false);
//	m_geoBatch[GeometryBatches_MechArms].SetVisibility(false);
//	m_geoBatch[GeometryBatches_RobotLying].SetVisibility(false);
//	m_geoBatch[GeometryBatches_RobotMoving].SetVisibility(false);
//	m_geoBatch[GeometryBatches_Passage].SetVisibility(false);
//
//	return true;
//
//	m_doorsAnim->Update(time / 1000.0f, sm::Matrix::IdentityMatrix(), ms / 1000.0f);
//
//	m_fovPower -= (ms / 1000.0f) * 2.0f;
//	if (m_fovPower < 0.0f)
//		m_fovPower = 0.0f;
//
//	//m_fovPower = 1.0f;
//
//	if (m_fovSignal->CheckSignal(time / 1000.0f))
//	{
//		m_fovPower = 1.0f;
//	}
//
//	//if ((time / 1000.0f) >= 120.0f && m_activeScene != m_breakingWallScene)
//	//{
//	//	m_activeScene->SetVisibility(false);
//	//	m_breakingWallScene->SetVisibility(true);
//
//	//	m_activeScene = m_breakingWallScene;
//	//}
//
//	/*if (m_assemblingScene->IsActiveOnTime(time / 1000.0f) && (m_activeScene != m_assemblingScene))
//	{
//		m_assemblingScene->SetVisibility(true);
//
//		m_activeScene = m_assemblingScene;
//	}*/
//
//	camerasAnimation->Update(time / 1000, sm::Matrix::IdentityMatrix(), ms / 1000.0f);
//
//	if (errorOccured)
//		return false;
//
//	m_activeScene->Update(time / 1000.0f, ms / 1000.0f);
//
//#ifndef MAN_CAM
//	cameraMode = CameraMode_Preview;
//#endif
//	if (cameraMode == CameraMode_Preview) 
//	{
//		currentCamera = animCamsMng.GetActiveCamera(time / 1000.0f);
//		assert(currentCamera != NULL);
//	}
//
//	if (camShakeVal == 0.0f)
//		view = currentCamera->GetViewMatrix();
//	else
//		view =
//			sm::Matrix::RotateAxisMatrix(random.GetFloat(0.0f, 0.1f) * camShakeVal, sm::Vec3(random.GetFloat(-1, 1), random.GetFloat(-1, 1), random.GetFloat(-1, 1)).GetNormalized()) *
//			currentCamera->GetViewMatrix();
//
//	//view = view * sm::Matrix::ScaleMatrix(0.05f, 0.05f, 0.05f);
//
//	float camfov = deg(currentCamera ->GetFov(time / 1000.0f));
//
//	if (fov != camfov)
//	{
//		fov = camfov;
//
//		proj = sm::Matrix::PerspectiveMatrix(
//			fov, (float)width / (float)height, NEAR_PLANE, FAR_PLANE);
//
//		/*glowProj = sm::Matrix::PerspectiveMatrix(
//			fov, (float)width / (float)height, NEAR_PLANE, FAR_PLANE);*/
//	}
//
//	if (dynamic_cast<ManCam*>(currentCamera) != NULL)
//		((ManCam*)currentCamera)->Process(ms);
//
//	//scene ->Update(time, ms);
//	//shadowPass->Update(time, ms);
//
//#ifndef DISABLE_FRUSTUM_CULLING
//	frustum->SetFrustum(view, NEAR_PLANE, FAR_PLANE * 100.0f, fov, (float)width / (float)height);
//	FrustumCulling(proj, view, sm::Matrix::IdentityMatrix(), allMeshParts);
//
//	for (uint32_t i = 0; i < GeometryBatches_Count; i++)
//	{
//		if (m_geoBatch[i].IsVisible())
//			FrustumCulling(proj, view, sm::Matrix::IdentityMatrix(), m_geoBatch[i].allMeshParts);
//	}
//#endif

	return true;
}

static float fadeDir = 0.05f;

void DemoController::InitElectroNoise()
{
	int noiseHeight = height / 4;

	float *textureData = new float[noiseHeight];
	for (int i = 0; i < noiseHeight; i++)
		textureData[i] = ((float)(rand() % 200) - 100.0f) / 100.0f;

	glGenTextures(1, &noiseTex);
	glBindTexture(GL_TEXTURE_1D, noiseTex);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage1D(GL_TEXTURE_1D, 0, 1, noiseHeight, 0, GL_LUMINANCE, GL_FLOAT, textureData);
	glBindTexture(GL_TEXTURE_1D, 0);

	delete [] textureData;
}

float DemoController::CalcFlash(float time, float ms)
{
	time /= 1000.0f;

	AnimCamera *cam =
		dynamic_cast<AnimCamera*>(currentCamera);

	if (cam == NULL)
		return 1.0f;

	if (time > cam->GetLastKeyFrameTime() - 0.3f)
		fadeDir = -1;

	fade = fade + ((ms / 300.0f) * fadeDir);

	if (fade >= 1.0f)
	{
		fade = 1.0f;
	}
	else if (fade <= 0.0f)
	{
		fade = 0.0f;
		fadeDir = 0.2f;
	}

	return fade;
}

bool DemoController::Draw(float time, float ms)
{
	float seconds = ms / 1000.0f;
	time /= 1000.0f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_robot->Draw(time, seconds);

	//DrawingRoutines::DrawDiffLight(m_teapot, viewProj, sm::Vec3(0, 0, 100));

	//glUseProgram(0);

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	proj = sm::Matrix::PerspectiveMatrix(60.0f, (float)width / (float)height, 0.125f, 100.0f);
	glLoadMatrixf(proj);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -10);


	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glColorMask(1, 1, 1, 1);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_TRIANGLES);
	
	glVertex3f(-10, -10, 0);
	glVertex3f(0, 10, 0);
	glVertex3f(10, -10, 0);
	glEnd();*/

	//********************

//	//return true;
//
//	time = lastTime;
//
//	sm::Matrix viewInv = view.GetInversed();
//	sm::Vec3 camPos(viewInv.a[12], viewInv.a[13], viewInv.a[14]);
//	sm::Vec3 camRight = sm::Vec3(viewInv.a[0], viewInv.a[1], viewInv.a[2]);
//	sm::Vec3 camTrg(-viewInv.a[8], -viewInv.a[9], -viewInv.a[10]);
//
//	//DrawGlows(ms, camPos, glowTex->GetId());
//
//	sm::Matrix lightTransform;
//
//	lightTransform.a[0] = -0.9239f;
//lightTransform.a[1] = 0.1557f;
//lightTransform.a[2] = -0.3496f;
//lightTransform.a[3] = 0.0000f;
//lightTransform.a[4] = 0.0000f;
//lightTransform.a[5] = 0.9135f;
//lightTransform.a[6] = 0.4067f;
//lightTransform.a[7] = 0.0000f;
//lightTransform.a[8] = 0.3827f;
//lightTransform.a[9] = 0.3758f;
//lightTransform.a[10] = -0.8440f;
//lightTransform.a[11] = 0.0000f;
//lightTransform.a[12] = -703.3286f;
//lightTransform.a[13] = -780.8308f;
//lightTransform.a[14] = -193.7107f;
//lightTransform.a[15] = 1.0000f;
//
//m_activeScene->GetLightTransform(lightTransform);
//
//
//	//sm::Vec3 lightPos = endPivot->AnimTransform() * sm::Vec3(0, 0, 0);
//	sm::Vec3 lightPos = camPos;
//
//	shadowPass->DrawLight(time, ms, lightTransform, proj);
//
//	framebuffer ->BindFramebuffer();
//	framebuffer ->AttachColorTexture(targetTex0 ->GetId());
//	framebuffer ->AttachDepthTexture(depthTex ->GetId());
//	framebuffer ->Validate();
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	for (uint32_t i = 0; i < GeometryBatches_Count; i++)
//	{
//		if (m_geoBatch[i].IsVisible())
//			drawingRoutines->DrawStandardLighting_GlowShadow(
//				m_geoBatch[i].solidMeshParts,
//				proj,
//				view,
//				sm::Matrix::IdentityMatrix(),
//				lightPos,
//				camPos,
//				lightTransform,
//				proj,
//				shadowPass->GetShadowMap());
//	}
//
//	//drawingRoutines ->DrawStandardLighting_GlowShadow(solidMeshParts, proj, view, sm::Matrix::IdentityMatrix(), lightPos, camPos, lightTransform, proj, shadowPass->GetShadowMap());
//
//	//m_activeScene->Draw(time / 1000.0f, ms / 1000.0f);
//	//m_robot->Draw(time / 1000.0f, ms / 1000.0f);
//	//electro->SetParameter("minIntensity", 0.1f, 0.1f, 0.1f, 1.0f);
//	//electro->SetParameter("minIntensity", 0.0f, 0.0f, 0.0f, 0.0f);
//
//	/*for (uint32_t i = 0; i < GeometryBatches_Count; i++)
//	{
//		if (m_geoBatch[i].IsVisible())
//			drawingRoutines->DrawStandardLighting_GlowShadow(
//				m_geoBatch[i].opacityMeshParts,
//				proj,
//				view,
//				sm::Matrix::IdentityMatrix(),
//				lightPos,
//				camPos,
//				lightTransform,
//				proj,
//				shadowPass->GetShadowMap());
//	}*/
//
//	//drawingRoutines ->DrawStandardLighting_GlowShadow(opacityMeshParts, proj, view, sm::Matrix::IdentityMatrix(), lightPos, camPos, lightTransform, proj, shadowPass->GetShadowMap());
//	//m_activeScene->DrawOpacities(time / 1000.0f, ms / 1000.0f);
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_ONE, GL_ONE);
//	//Utils::DrawSprite(blur->GetBlurredTexture(2), 0, 0, width, height);
//	glDisable(GL_BLEND);
//	Framebuffer::RestoreDefaultFramebuffer();
//
//	//dofBlur->MakeBlur(targetTex0->GetId(), 1, false);
//
//	//DrawPostProcess(targetTex0->GetId(), dofBlur->GetBlurredTexture(2), 0, 0, m_fovPower);
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glColor4f(1, 1, 1, 1);
//	//Utils::DrawSprite(mask->GetId(), 0, 0, width, height);
//	glDisable(GL_BLEND);
//
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef SHOW_FPS
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1, 0, 0, 1);
	float fps= CalcFps(ms);
	char fpsText[1024];
	sprintf(fpsText, "fps: %.2f", fps);
	DrawText(fpsText, 4, height - 20, 255, 0, 0);
	DrawEngineStats();

	sprintf(fpsText, "fade: %.2f", fade);
	DrawText(fpsText, 4, height - 140, 255, 0, 0);
	sprintf(fpsText, "time: %.2f", time / 1000.0f);
	DrawText(fpsText, 4, height - 160, 255, 0, 0);
#endif

	/*if (m_boardOpacity > 0.0f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
		glColor4f(1, 1, 1, m_boardOpacity);
		Utils::DrawSprite(
			m_boardTexs[m_boardIndex]->GetId(),
			0,
			0,
			width,
			height);
	}*/

	glWnd ->SwapBuffers();

	return true;
}

void DemoController::DrawPostProcess(
	int sharpTexId,
	int blurTexId,
	int glowTex,
	unsigned int shadowTex,
	float fade)
{
	//Effect *pp = DemoContent::GetInstance() ->Get<Effect*>("post_process");
	//sm::Matrix mvp = sm::Matrix::Ortho2DMatrix(0, (float)width, 0, (float)height);

	//glBindTexture(GL_TEXTURE_2D, 0);
	//pp ->SetCurrentTechnique("DofGlowTech");

	//pp ->SetParameter("focal", 100.0f);
	//pp ->SetParameter("fade", fade);
	//pp ->SetParameter("focalFalloff", 0.00000007f);
	//pp->SetParameter("noiseStrength", electroNoiseVal);

	//pp ->SetTextureParameter("depthTex", depthTex ->GetId());
	//pp ->SetTextureParameter("sharpTex", sharpTexId);
	//pp ->SetTextureParameter("blurTex", blurTexId);
	//pp ->SetTextureParameter("noiseTex", noiseTex);
	//pp ->SetParameter("mvp", mvp);
	//pp ->BeginPass(0);
	
	//glBegin(GL_QUADS);
	//glTexCoord2i(0, 0); glVertex2i(0, 0);
	//glTexCoord2i(0, 1); glVertex2i(0, height);
	//glTexCoord2i(1, 1); glVertex2i(width, height);
	//glTexCoord2i(1, 0); glVertex2i(width, 0);
	//glEnd();

	//pp ->EndPass(0);
}

void DemoController::SetOpenglParams()
{
	int width = glWnd ->GetWindowWidth();
	int height = glWnd ->GetWindowHeight();

	wglUseFontBitmaps(glWnd ->GetHDC(), 0, 255, 1000);
	glListBase(1000);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	proj = sm::Matrix::PerspectiveMatrix(fov, (float)width / (float)height, NEAR_PLANE, FAR_PLANE);
	//glowProj = sm::Matrix::PerspectiveMatrix(fov, (float)(width / 4) / (float)(height / 4), NEAR_PLANE, FAR_PLANE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_NORMALIZE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

bool DemoController::AssignAssets()
{
	bool result = true;

	m_content->CombineResources();

	return result;
}

void DemoController::ErrorOccured( const std::string &errorMsg )
{
	Log::LogT("error: %s", errorMsg.c_str());
}

void DemoController::OnLoadingContentStarted(int stepsCount)
{
	loadingScreen ->SetStepsCount(stepsCount);
	loadingScreen ->Update(0, 0);
	loadingScreen ->Draw(0, 0, view, proj, sm::Vec3(0, 0, 0));
}

void DemoController::OnProgressStep()
{
	loadingScreen ->ProgressStep();

	loadingScreen ->Update(0, 0);
	loadingScreen ->Draw(0, 0, view, proj, sm::Vec3(0, 0, 0));
}

void DemoController::OnLoadingFinished()
{
}

bool DemoController::BeforeStartSetups()
{
	Content *dc = m_content;

	DrawingRoutines::Initialize(m_content);

	/*Shader *blurEffect = dc->Get<Shader>("blur");
	assert(blurEffect != NULL);
	blur = new Blur(3, blurEffect, width / 8, height / 8);
	dofBlur = new Blur(3, blurEffect, width / 4, height / 4);

	std::vector<Model*> models;*/

	//models.push_back(m_mdl_teapot);
	//models.push_back(m_mdl_factory);

	//models.push_back(m_doors);

	/*std::vector<Model*> &sceneModels1 = m_assemblingScene->GetModels();
	models.insert(models.end(), sceneModels1.begin(), sceneModels1.end());
	std::vector<Model*> &sceneModels2 = m_breakingWallScene->GetModels();
	models.insert(models.end(), sceneModels2.begin(), sceneModels2.end());*/

	//models.push_back(m_mdl_korytarz);
	
	//for (unsigned i = 0; i < models.size(); i++)
	//{
	//	std::vector<Mesh*> &meshes = models[i]->GetMeshes();
	//	for (unsigned j = 0; j < meshes.size(); j++)
	//	{
	//		std::vector<MeshPart*> &meshParts = meshes[j]->GetMeshParts();
	//		allMeshParts.insert(allMeshParts.begin(), meshParts.begin(), meshParts.end());
	//	}
	//}

	//FilterGlowObjects(models, glowMeshParts, nonGlowMeshParts);
	//FilterOpacityObjects(models, opacityMeshParts, solidMeshParts);
	//SetAlwaysVisibility(models);

	//for (uint32_t i = 0; i < GeometryBatches_Count; i++)
	//{
	//	FilterGlowObjects(m_geoBatch[i].m_models, m_geoBatch[i].glowMeshParts, m_geoBatch[i].nonGlowMeshParts);
	//	FilterOpacityObjects(m_geoBatch[i].m_models, m_geoBatch[i].opacityMeshParts, m_geoBatch[i].solidMeshParts);
	//}

	////shadowPass = new ShadowMappingTest(width, height);
	////shadowPass->Initialize();

	return true;
}

int DemoController::GetNextId()
{
	nextId++;
	return nextId;
}

void DemoController::DrawText(const std::string &text, int x, int y, BYTE r, BYTE g, BYTE b)
{
	//m_fontRenderer->DrawString("loda", 10, 10, Color::Blue);

	// ustaw macierz na ortho

	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(sm::Matrix::Ortho2DMatrix(0, width, height, 0));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glRasterPos2d(x, y);

	glColor3ub(r, g, b);
	glCallLists((int)text.size(), GL_UNSIGNED_BYTE, text.c_str());

	glPopAttrib();
}

float DemoController::CalcFps(float ms)
{
	flaps++;

	delay += ms;
	if (delay > delayLimit)
	{
		fps = (float)flaps / delay * 1000.0f;
		delay = 0.0f;
		flaps = 0;
	}

	return fps;
}

void DemoController::FilterGlowObjects(const std::vector<Model*> &models,
									   std::vector<MeshPart*> &glowMeshParts,
									   std::vector<MeshPart*> &nonGlowMeshParts)
{
	for (unsigned i = 0; i < models.size(); i++)
	{
		std::vector<Mesh*> &meshes = models[i] ->GetMeshes();

		for (unsigned j = 0; j < meshes.size(); j++)
		{
			std::vector<MeshPart*> &meshParts = meshes[j] ->GetMeshParts();

			for (unsigned k = 0; k < meshParts.size(); k++)
			{
				Material *mat = meshParts[k] ->GetMaterial();

				if (mat != NULL &&
					mat ->name.size() >= 4 &&
					mat ->name.substr(0, 4) == "glow")
					glowMeshParts.push_back(meshParts[k]);
				else
					nonGlowMeshParts.push_back(meshParts[k]);
			}
		}
	}
}


void DemoController::FilterOpacityObjects(const std::vector<Model*> &models,
										  std::vector<MeshPart*> &opacityMeshParts,
										  std::vector<MeshPart*> &solidGlowMeshParts)
{
	for (unsigned i = 0; i < models.size(); i++)
	{
		std::vector<Mesh*> &meshes = models[i] ->GetMeshes();

		for (unsigned j = 0; j < meshes.size(); j++)
		{
			std::vector<MeshPart*> &meshParts = meshes[j] ->GetMeshParts();

			for (unsigned k = 0; k < meshParts.size(); k++)
			{
				Material *mat = meshParts[k] ->GetMaterial();

				if (mat != NULL &&
					mat ->opacity < 1.0)
					opacityMeshParts.push_back(meshParts[k]);
				else
					solidGlowMeshParts.push_back(meshParts[k]);
			}
		}
	}
}

void DemoController::DrawGlows(
	float ms,
	const sm::Vec3 &camPos,
	unsigned int texId)
{
	//blurFbo->BindFramebuffer();
	//blurFbo->AttachColorTexture(texId);
	//blurFbo->Validate();

	//glViewport(0, 0, width / 4, height / 4);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//for (uint32_t i = 0; i < GeometryBatches_Count; i++)
	//{
	//	if (m_geoBatch[i].IsVisible())
	//		drawingRoutines->DrawStandardLighting(m_geoBatch[i].glowMeshParts, proj, view, sm::Matrix::IdentityMatrix(), camPos, camPos);
	//}

	//drawingRoutines->DrawStandardLighting(glowMeshParts, proj, view, sm::Matrix::IdentityMatrix(), camPos, camPos);
	////m_sparksGenerator.Draw(ms / 1000.0f, proj, view);
	////m_mechArm1->DrawSparks(ms / 1000.0f, proj, view);

	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	//m_activeScene->DrawGlows(0.0f, ms / 1000.0f);

	//for (uint32_t i = 0; i < GeometryBatches_Count; i++)
	//{
	//	if (m_geoBatch[i].IsVisible())
	//		drawingRoutines->DrawGlowMask(m_geoBatch[i].nonGlowMeshParts, proj, view, sm::Matrix::IdentityMatrix());
	//}

	//drawingRoutines->DrawGlowMask(nonGlowMeshParts, proj, view, sm::Matrix::IdentityMatrix());

	//if (m_boardOpacity > 0.0f)
	//{
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//	glColor4f(1, 1, 1, m_boardOpacity);
	//	Utils::DrawSprite(
	//		m_boardTexs[m_boardIndex]->GetId(),
	//		0,
	//		0,
	//		width / 4,
	//		height / 4);
	//}

	//Framebuffer::RestoreDefaultFramebuffer();

	//glViewport(0, 0, width, height);

	//blur->MakeBlur(texId, 1, true);
}

void DemoController::FrustumCulling(
		const sm::Matrix &proj,
		const sm::Matrix &view,
		const sm::Matrix &world,
		std::vector<MeshPart*> &meshParts)
{
	for (unsigned i = 0; i < meshParts.size(); i++)
	{
		if (meshParts[i]->IsAlvaysVisible())
			continue;

		if (frustum->IsSphereInside(meshParts[i]->bsphere->center, meshParts[i]->bsphere->radius, *meshParts[i]->bbox))
			meshParts[i]->SetVisibility(true);
		else
			meshParts[i]->SetVisibility(false);
	}
}

void DemoController::DrawEngineStats()
{
	unsigned totalVerticesCount = 0;
	unsigned visibleVerticesCount = 0;
	unsigned totalMeshPartsCount = allMeshParts.size();
	for (uint32_t i = 0; i < GeometryBatches_Count; i++)
	{
		if (m_geoBatch[i].IsVisible())
			totalMeshPartsCount += m_geoBatch[i].allMeshParts.size();
	}
	unsigned visibleMeshPartsCount = 0;

	for (unsigned i = 0; i < allMeshParts.size(); i++)
	{
		totalVerticesCount += allMeshParts[i]->GetVerticesCount();

		if (allMeshParts[i]->IsVisible())
		{
			visibleMeshPartsCount++;
			visibleVerticesCount += allMeshParts[i]->GetVerticesCount();
		}
	}

	for (uint32_t i = 0; i < GeometryBatches_Count; i++)
	{
		for (unsigned j = 0; j < m_geoBatch[i].allMeshParts.size(); j++)
		{
			if (m_geoBatch[i].IsVisible())
			{
				totalVerticesCount += m_geoBatch[i].allMeshParts[j]->GetVerticesCount();

				if (m_geoBatch[i].allMeshParts[j]->IsVisible())
				{
					visibleMeshPartsCount++;
					visibleVerticesCount += m_geoBatch[i].allMeshParts[j]->GetVerticesCount();
				}
			}
		}
	}

	char text[1024];
	sprintf(text, "visible meshes: %u / %u (%.1f%%)", visibleMeshPartsCount, totalMeshPartsCount,
		100.0f * (float)visibleMeshPartsCount / (float)totalMeshPartsCount);
	DrawText(text, 4, height - 40, 255, 0, 0);
	sprintf(text, "visible vertices: %u / %u (%.1f%%)", visibleVerticesCount, totalVerticesCount,
		100.0f * (float)visibleVerticesCount / (float)totalVerticesCount);
	DrawText(text, 4, height - 60, 255, 0, 0);
}

void DemoController::SetAlwaysVisibility(const std::vector<Model*> &models)
{
	for (unsigned i = 0; i < models.size(); i++)
	{
		std::vector<Mesh*> &meshes = models[i] ->GetMeshes();

		for (unsigned j = 0; j < meshes.size(); j++)
		{
			if (meshes[j]->name.size() >= 4 &&
				meshes[j]->name.substr(0, 7) == "credits")
			{
				for (unsigned k = 0; k < meshes[j]->GetMeshParts().size(); k++)
				{
					meshes[j]->GetMeshParts()[k]->IsAlvaysVisible() = true;
					//meshes[j]->colorMask = sm::Vec3(2, 2, 2);
				}
			}
		}
	}
}

int propVal = 1;
static float timeShift = -1;

void DemoController::OnLeftMouseDown()
{
	if (timeShift == -1)
		timeShift = lastTime / 1000.0f;

	Log::LogT("changeGreetzProp->SetValue(%d, %.4f);", propVal, (lastTime / 1000.0f) - 0);

	if (propVal == 1)
		propVal = 0;
	else
		propVal = 1;
}

static int beatPropVal;

void DemoController::OnKeyDown(int keyCode)
{
	switch (keyCode)
	{
	case 'L':
		Log::LogT("Light");
		for (uint32_t i = 0; i < 16; i++)
		{
			Log::LogT("lightTransform.a[%d] = %.4ff;", i, view.a[i]);
		}
		Log::LogT("");
		break;

	case 'B':
		if (beatPropVal == 0)
			beatPropVal = 1;
		else
			beatPropVal = 0;

		Log::LogT("m_fovAnim->SetValue(%d, %.4f);", beatPropVal, (lastTime / 1000.0f));
		break;
	}
}

Texture* DemoController::LoadTexture(const std::string &path)
{
	return TextureLoader::Load(path);
}

Shader* DemoController::LoadShader(const std::string &vertexShaderPath,
								      const std::string &fragmentShaderPath)
{
	Shader *shader = Shader::LoadFromFile(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

	return shader;
}

Model* DemoController::LoadModel(const std::string &path)
{
	return ModelLoader::LoadFromFile(path);
}

Animation* DemoController::LoadAnimation(const std::string &path)
{
	return AnimationLoader::LoadFromFile(path);
}

Material* DemoController::LoadMaterial(const std::string &path)
{
	return MaterialLoader::LoadFromFile(path);
}