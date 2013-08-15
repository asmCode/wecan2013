#pragma once

#include "DemoMode.h"
#include "CameraMode.h"
#include "GraphicsLibrary\CubeTexture.h"

#include <Core/GenericSingleton.h>

#include <Graphics/Content/Content.h>
#include "Music.h"

#include <Graphics/Interpolators/InterpolatorFactory.h>
#include <Graphics/IGraphicsEngine.h>
#include "GraphicsLibrary/AnimCamera.h"
#include <Graphics/Animation.h>

#include "GraphicsLibrary\OpenglWindow.h"
#include "GraphicsLibrary\SteerCamera.h"
#include "GraphicsLibrary\IKeyboardCallback.h"

#include "SparksGenerator.h"
#include "GeometryBatch.h"

#include "IScene.h"
#include "ManCam.h"
#include "AnimCameraManager.h"
#include <Graphics/Interpolators/Interpolator.h>
#include <Graphics/Interpolators/InterpolatorFactory.h>
#include <Graphics/Content/IContentObserver.h>
#include <Utils/Randomizer.h>

#include <windows.h>
#include <vector>
#include <map>

DWORD WINAPI EditableDemoThread(void *params);
class DemoController;
class EditableDemo;
class MachineScreen;
class EditableCamera;
class ManCam;
class ShadowMappingTest;
class LoadingScreen;
class Framebuffer;
class DepthTexture;
class Blur;
class PostProcessing;
class Frustum;
class Screens;
class BreakingWallScene;
class AssemblingScene;
class Robot;
class PropertySignal;
class Content;
class SpriteBatch;
class FontRenderer;

#define demo DemoController::GetInstance()

class DemoController :
	public IContentObserver,
	public IKeyboardCallback,
	public IGraphicsEngine,
	public GenericSingleton<DemoController>
{
	friend class GenericSingleton<DemoController>;
	friend class NormalMappingTest;
	friend class ShadowMappingTest;
	friend class LoadingScreen;
	friend class MachineScreen;

public:

	Content *m_content;

	enum GeometryBatches
	{
		GeometryBatches_FactoryHall = 0,
		GeometryBatches_Passage,
		GeometryBatches_RobotLying,
		GeometryBatches_RobotMoving,
		GeometryBatches_MechArms,
		GeometryBatches_Count
	};

	GeometryBatch m_geoBatch[GeometryBatches_Count];

	std::string m_strBasePath;

	PropertySignal *m_fovSignal;

	float m_fovPower;

	IScene *m_activeScene;

	Model *m_mdl_teapot;

	Model *m_doors;
	Animation *m_doorsAnim;

	SparksGenerator m_sparksGenerator;

	SpriteBatch *m_spriteBatch;
	FontRenderer *m_fontRenderer;
	Shader *m_spriteShader;
	
	//Effect *electro;

	static Randomizer random;

	unsigned noiseTex;
	void InitElectroNoise();

	IInterpolator<float> *eyeBlinkAnim;
	IInterpolator<float> *eyeRangeAnim;

	float eyeBlinkVal;
	float eyeRangeVal;
	float electroNoiseVal;
	float camShakeVal;

	ICamera *currentCamera;
	AnimCameraManager animCamsMng;

	float fade;

	Robot *m_robot;

	float CalcFlash(float time, float ms);

	bool isPlaying;

	Texture *mask;

	bool errorOccured;
	DemoMode demoMode;
	MachineScreen *machineScreen;

	OpenglWindow *glWnd;
	ManCam manCam;

	Animation *camerasAnimation;

	Frustum *frustum;
	sm::Matrix view;
	sm::Matrix proj;
	sm::Matrix m_viewProj;
	sm::Matrix glowProj;
	Music music;
	bool isStereo;

	PostProcessing *postProcessing;

	Framebuffer *framebuffer;
	Framebuffer *blurFbo;
	Texture *targetTex0;
	Texture *glowTex;
	DepthTexture *depthTex;

	Blur *blur;
	Blur *dofBlur;

	CubeTexture *m_envTexture;

	std::vector<MeshPart*> allMeshParts;
	std::vector<MeshPart*> glowMeshParts;
	std::vector<MeshPart*> nonGlowMeshParts;
	std::vector<MeshPart*> opacityMeshParts;
	std::vector<MeshPart*> solidMeshParts;

	ShadowMappingTest *shadowPass;
	IScene *scene;
	LoadingScreen *loadingScreen;

	CameraMode cameraMode;

	int nextId;

	float fov;

	DemoController();
	~DemoController();

	void SetOpenglParams();
	bool InitCg();

	void InitializeProperties();
	bool AssignAssets();

	// IContentObserver interface
	void ErrorOccured(const std::string &errorMsg);
	void OnLoadingContentStarted(int stepsCount);
	void OnProgressStep();
	void OnLoadingFinished();

	template <typename T>
	void DeleteObject(T &obj)
	{
		if (obj != NULL)
		{
			delete obj;
			obj = NULL;
		}
	}

	void DrawText(const std::string &text, int x, int y, BYTE r = 255, BYTE g = 255, BYTE b = 255);
	float CalcFps(float ms);
	void DrawEngineStats();

	int width;
	int height;

	int flaps;
	float delay;
	float delayLimit;
	float fps;

	Model *m_teapot;

	BreakingWallScene *m_breakingWallScene;
	AssemblingScene *m_assemblingScene;

	void DrawGlows(
		float ms,
		const sm::Vec3 &camPos,
		unsigned int texId);

	void FrustumCulling(
		const sm::Matrix &proj,
		const sm::Matrix &view,
		const sm::Matrix &world,
		std::vector<MeshPart*> &meshParts);

public:
	void OnLeftMouseDown();
	void OnKeyDown(int keyCode);

	bool demoEnded;

	bool Initialize(bool isStereo, DemoMode demoMode, HWND parent, const char *title, int width, int height,
		int bpp, int freq, bool fullscreen, bool createOwnWindow);
	bool LoadContent(const char *basePath);
	bool BeforeStartSetups();
	void Release();

	bool Update(float time, float ms);
	bool Draw(float time, float ms);

	int GetNextId();

	void LoadProperties(const std::string &filename);

	void FilterGlowObjects(
		const std::vector<Model*> &models,
		std::vector<MeshPart*> &glowMeshParts,
		std::vector<MeshPart*> &nonGlowMeshParts);

	void SetAlwaysVisibility(
		const std::vector<Model*> &models);

	void FilterOpacityObjects(
		const std::vector<Model*> &models,
		std::vector<MeshPart*> &opacityMeshParts,
		std::vector<MeshPart*> &solidGlowMeshParts);

	void DrawPostProcess(int sharpTexId, int blurTexId, int glowTex, unsigned int shadowTex, float fade);

	Texture* LoadTexture(const std::string &path);
	Shader* LoadShader(const std::string &vertexShaderPath,
										const std::string &fragmentShaderPath);
	Model* LoadModel(const std::string &path);
	Animation* LoadAnimation(const std::string &path);
	Material* LoadMaterial(const std::string &path);
};
