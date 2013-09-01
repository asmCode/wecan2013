#include "DemoController.h"
#include <Utils/Log.h>
#include "BasicLightingEffect.h"
#include "ShadowMappingTest.h"
#include <Graphics/Framebuffer.h>
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
#include "Billboard.h"
#include "DistortParticleHandler.h"
#include "Particles/ParticleEmmiter.h"
#include "Particles/IParticleHandler.h"

#include <Graphics/TextureLoader.h>
#include <Graphics/ModelLoader.h>
#include <Graphics/MaterialLoader.h>
#include <Graphics/Shader.h>
#include <Graphics/AnimationLoader.h>
#include <Graphics/SpriteBatch.h>
#include <Graphics/FontRenderer.h>

#include <algorithm>

const float DemoController::GlowBufferWidthRatio = 0.5f;
const float DemoController::GlowBufferHeightRatio = 0.5f;

//#define DISABLE_MUSIC 1
#define DISABLE_FRUSTUM_CULLING 1
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
	m_fovPower(0.0f),
	m_particleEmmiter(NULL),
	m_distortParticleHandler(NULL)
{
	fade = 0.0f;
	blurFbo = NULL;
	dofBlur = NULL;
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

	Log::StartLog(true, true, false);
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

void DemoController::AssignLightmapsToModels()
{
	for (uint32_t i = 0; i < allMeshParts.size(); i++)
	{
		MeshPart *meshPart = allMeshParts[i];

		std::string lightmapName = allMeshParts[i]->mesh->name + "LightingMap";
		Texture *lightmap = m_content->Get<Texture>(lightmapName);

		if (lightmap != NULL &&
			!(allMeshParts[i]->material != NULL &&
			  allMeshParts[i]->material->name.size() >= 4 &&
			  allMeshParts[i]->material->name.substr(0, 4) == "glow"))
			  allMeshParts[i]->m_lightmap = lightmap;

		if (allMeshParts[i]->material != NULL &&
			  allMeshParts[i]->material->name.size() >= 4 &&
			  allMeshParts[i]->material->name.substr(0, 4) == "glow")
		{
			int a = 9;
		}
	}
}

void DemoController::InitializeBlur()
{
	uint32_t glowWidth = static_cast<uint32_t>(width * GlowBufferWidthRatio);
	uint32_t glowHeight = static_cast<uint32_t>(height * GlowBufferHeightRatio);

	m_glowTex = new Texture(
		glowWidth,
		glowHeight,
		32,
		NULL,
		Texture::Wrap_ClampToEdge, Texture::Filter_Linear,
		Texture::Filter_Linear, false); 

	m_horiBlurShader = m_content->Get<Shader>("HoriBlur");
	assert(m_horiBlurShader != NULL);
	m_horiBlurShader->BindVertexChannel(0, "a_position");
	m_horiBlurShader->BindVertexChannel(1, "a_coords");
	m_horiBlurShader->LinkProgram();

	m_vertBlurShader = m_content->Get<Shader>("VertBlur");
	assert(m_vertBlurShader != NULL);
	m_vertBlurShader->BindVertexChannel(0, "a_position");
	m_vertBlurShader->BindVertexChannel(1, "a_coords");
	m_vertBlurShader->LinkProgram();

	m_glowFramebuffer = new Framebuffer();
	m_glowFramebuffer->Initialize(glowWidth, glowHeight, 32);
	m_glowBlur = new Blur(1, m_horiBlurShader, m_vertBlurShader, glowWidth, glowHeight);
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

	blurFbo = new Framebuffer();
	blurFbo ->Initialize(width / 2, height / 2, 32);

	depthTex = new DepthTexture(demo ->width, demo ->height);
	depthTex->BindTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
	framebuffer ->AttachDepthTexture(depthTex ->GetId());

	Billboard::Initialize();

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
	dc->LoadTextures(m_strBasePath + "textures\\lightmaps\\");
	dc->LoadShaders(m_strBasePath + "effects\\");
	dc->LoadAnimations(m_strBasePath + "animations\\");
	dc->LoadMaterials(m_strBasePath + "materials\\");

	if (!AssignAssets())
		return false;

	InitializeBlur();

	m_spriteShader = m_content->Get<Shader>("Sprite");
	assert(m_spriteShader != NULL);
	m_spriteShader->BindVertexChannel(0, "a_position");
	m_spriteShader->BindVertexChannel(0, "a_coords");
	m_spriteShader->LinkProgram();

	m_spriteBatch = new SpriteBatch(m_spriteShader, sm::Matrix::Ortho2DMatrix(0, width, 0, height));
	m_fontRenderer = FontRenderer::LoadFromFile((m_strBasePath + std::string("fonts\\komika_title_32.xml")).c_str(), m_spriteBatch);

	Shader *distortShader = m_content->Get<Shader>("DistortParticle");
	assert(distortShader != NULL);
	distortShader->BindVertexChannel(0, "a_position");
	distortShader->LinkProgram();

	m_particleTex = m_content->Get<Texture>("smoke2");
	assert(m_particleTex != NULL);
	m_distortParticleTex = m_content->Get<Texture>("smoke2_distort");
	assert(m_distortParticleTex != NULL);

	m_distortParticleHandler = new DistortParticleHandler(distortShader, m_particleTex, m_distortParticleTex);
	m_particleEmmiter = new ParticleEmmiter(1000, m_distortParticleHandler);
	m_particleEmmiter->SetSparksPerSecond(10);
	m_particleEmmiter->SetLifeTime(3.0f, 8.0f);
	m_particleEmmiter->SetSizeOverLifetime(1.0f, 10.0f);
	m_particleEmmiter->SetGravityVelocity(sm::Vec3(0, 3, 0));
	m_particleEmmiter->SetColorOverLifetime(sm::Vec4(1, 1, 1, 0.5), sm::Vec4(1, 1, 1, 0));
	m_particleEmmiter->SetSpeedOverLifetime(4.0f, 0.0f);
	m_particleEmmiter->SetSourceDirection(sm::Vec3(5, 0, 0), 2);

	anim = dc->Get<Animation>("animacja");
	Animation *headAnim = anim->GetAnimationByNodeName("Head");

	m_teapots = m_content->Get<Model>("teapots");
	assert(m_teapots != NULL);

	for (uint32_t i = 0; i < m_teapots->m_meshParts.size(); i++)
	{
		m_teapots->m_meshParts[i]->mesh->Transform() = sm::Matrix::ScaleMatrix(0.01f, 0.01f, 0.01f);
	}

	m_robot = new Robot();
	m_robot->Initialize(m_content);

	std::vector<Model*> allModels;
	m_content->GetAll<Model>(allModels);
	for (uint32_t i = 0; i < allModels.size(); i++)
		allModels[i]->GetMeshParts(allMeshParts);

	SortByOpacity(allMeshParts);

	FilterGlowObjects();

	AssignLightmapsToModels();

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

	Billboard::Release();

	DeleteObject(depthTex);
	DeleteObject(m_glowTex);
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

	m_activeCamera = NULL;

#if MAN_CAM
	manCam.Process(ms);
	m_activeCamera = &manCam;
#else
	camerasAnimation->Update(time, sm::Matrix::IdentityMatrix(), seconds);
	m_activeCamera = animCamsMng.GetActiveCamera(time);
#endif	

	m_proj = sm::Matrix::PerspectiveMatrix((m_activeCamera->GetFov(time) / 3.1415f) * 180.0f, (float)width / (float)height, 0.1f, 100.0f);
	m_view = m_activeCamera->GetViewMatrix();
	m_viewProj = m_proj * m_view;

	//anim->Update(time / 1000.0f, sm::Matrix::IdentityMatrix(), seconds);

	//m_robot->SetViewProjMatrix(m_viewProj);
	//m_robot->Update(time, seconds);

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
#ifndef DISABLE_FRUSTUM_CULLING
	frustum->SetFrustum(m_activeCamera->GetViewMatrix(), 0.1f, 100.0f, m_activeCamera->GetFov(time), (float)width / (float)height);
	FrustumCulling(allMeshParts);
#endif

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

Particle *ddd = NULL;

bool DemoController::Draw(float time, float ms)
{
	float seconds = ms / 1000.0f;
	time /= 1000.0f;

	glDepthMask(true);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawingRoutines::SetViewProjMatrix(m_viewProj);
	DrawingRoutines::SetLightPosition(sm::Vec3(0, 100, 100));
	DrawingRoutines::SetEyePosition(m_activeCamera->GetPosition());
	DrawingRoutines::SetLightPosition(m_activeCamera->GetPosition());

	//m_robot->Draw(time, seconds);

	DrawingRoutines::DrawWithMaterial(allMeshParts);

	/*if (ddd == NULL)
		ddd = m_distortParticleHandler->CreateParticle();

	ddd->m_position = sm::Vec3(0, 0, 0);
	ddd->m_size = 4.0f;
	ddd->m_color = sm::Vec4(1, 1, 1, 1);

	m_distortParticleHandler->SetMetrices(m_view, m_proj);
	m_distortParticleHandler->Setup();
	m_distortParticleHandler->Draw(ddd);
	m_distortParticleHandler->Clean();*/

	m_particleEmmiter->SetViewMatrix(m_view);
	m_particleEmmiter->SetProjMatrix(m_proj);

	m_particleEmmiter->Update(seconds);
	m_particleEmmiter->Draw(seconds);

	RenderGlowTexture();

	glViewport(0, 0, width, height);
	m_spriteBatch->Begin();
	glBlendFunc(GL_ONE, GL_ONE);
	m_spriteBatch->Draw(m_glowBlur->GetBlurredTexture(0), 0, 0, width, height);
	m_spriteBatch->End();

#ifdef SHOW_FPS
	glViewport(0, 0, width, height);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glColor4f(1, 0, 0, 1);
	float fps= CalcFps(ms);
	char fpsText[1024];
	sprintf(fpsText, "fps: %.2f", fps);
	DrawText(fpsText, 4, height - 20, 255, 0, 0);
	DrawEngineStats();

	sprintf(fpsText, "fade: %.2f", fade);
	DrawText(fpsText, 4, 0, 255, 0, 0);

	sm::Vec3 camPos = m_activeCamera->GetPosition();
	sprintf(fpsText, "camera position: (%.4f, %.4f, %.4f)", camPos.x, camPos.y, camPos.z);
	DrawText(fpsText, 4, 20, 255, 255, 255);

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

void DemoController::SetOpenglParams()
{
	int width = glWnd ->GetWindowWidth();
	int height = glWnd ->GetWindowHeight();

	wglUseFontBitmaps(glWnd ->GetHDC(), 0, 255, 1000);
	glListBase(1000);

	glViewport(0, 0, width, height);

	//glowProj = sm::Matrix::PerspectiveMatrix(fov, (float)(width / 4) / (float)(height / 4), NEAR_PLANE, FAR_PLANE);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

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
	//loadingScreen ->Draw(0, 0, view, proj, sm::Vec3(0, 0, 0));
}

void DemoController::OnProgressStep()
{
	loadingScreen ->ProgressStep();

	loadingScreen ->Update(0, 0);
	//loadingScreen ->Draw(0, 0, view, proj, sm::Vec3(0, 0, 0));
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

bool DemoController::HasGlowMaterial(MeshPart *meshPart)
{
	assert(meshPart != NULL);

	return
		meshPart->material != NULL &&
		meshPart->material->name.size() >= 4 &&
		meshPart->material->name.substr(0, 4) == "glow";
}

bool DemoController::HasOpacityMaterial(MeshPart *meshPart)
{
	assert(meshPart != NULL);

	return
		meshPart->material != NULL &&
		meshPart->material->opacity < 1.0f;
}

void DemoController::DrawText(const std::string &text, int x, int y, BYTE r, BYTE g, BYTE b)
{
	//m_fontRenderer->DrawString("abcdefgh", 10, 10, Color::Blue);

	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(sm::Matrix::Ortho2DMatrix(0, width, 0, height));
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

bool MeshPartComp(MeshPart *&a, MeshPart *&b)
{
	float aOpacity = 1.0f;
	float bOpacity = 1.0f;

	if (a->material != NULL)
		aOpacity = a->material->opacity;

	if (b->material != NULL)
		bOpacity = b->material->opacity;

	return aOpacity > bOpacity;
}

void DemoController::SortByOpacity(std::vector<MeshPart*> &meshParts)
{
	std::sort(meshParts.begin(), meshParts.end(),  MeshPartComp);
}

void DemoController::FilterGlowObjects()
{
	for (unsigned i = 0; i < allMeshParts.size(); i++)
	{
		MeshPart *mp = allMeshParts[i];

		if (HasGlowMaterial(mp))
		{
			if (HasOpacityMaterial(mp))
			{
				m_opacityGlowObjects.push_back(mp);
			}
			else
			{
				m_solidGlowObjects.push_back(mp);
			}
		}
		else
		{
			if (HasOpacityMaterial(mp))
			{
				m_opacityNonGlowObjects.push_back(mp);
			}
			else
			{
				m_solidNonGlowObjects.push_back(mp);
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

void DemoController::RenderGlowTexture()
{
	glViewport(0, 0, width * GlowBufferWidthRatio, height * GlowBufferHeightRatio);

	m_glowFramebuffer->BindFramebuffer();
	m_glowFramebuffer->AttachColorTexture(m_glowTex->GetId());
	m_glowFramebuffer->Validate();

	glDepthMask(true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawingRoutines::DrawWithMaterial(m_solidGlowObjects);
	DrawingRoutines::DrawBlack(m_solidNonGlowObjects);

	DrawingRoutines::DrawWithMaterial(m_opacityGlowObjects);
	DrawingRoutines::DrawBlack(m_opacityNonGlowObjects);

	Framebuffer::RestoreDefaultFramebuffer();

	m_glowBlur->MakeBlur(m_glowTex->GetId());
	
#if 0
	glViewport(0, 0, width, height);
	m_spriteBatch->Begin();
	glDisable(GL_BLEND);
	m_spriteBatch->Draw(m_glowBlur->GetBlurredTexture(0), 0, 0);
	m_spriteBatch->End();
#endif
}

void DemoController::FrustumCulling(std::vector<MeshPart*> &meshParts)
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
			Log::LogT("lightTransform.a[%d] = %.4ff;", i, m_view.a[i]);
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