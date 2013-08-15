//#pragma once
//
//#include "IScene.h"
//#include <Math\Matrix.h>
//
//#include <vector>
//#include <stdint.h>
//
//class Model;
//class Mesh;
//class Texture;
//class PropertySignal;
//class MechArm;
//class Animation;
//class Robot;
//
//class AssemblingScene : public IScene
//{
//public:
//	AssemblingScene(void);
//	~AssemblingScene(void);
//
//	bool Initialize();
//
//	void Update(float time, float seconds);
//	void Draw(float time, float seconds);
//	void DrawGlows(float time, float seconds);
//	void DrawOpacities(float time, float seconds);
//
//	void SetVisibility(bool visible);
//
//	bool IsActiveOnTime(float time);
//
//	std::vector<Model*>& GetModels();
//
//	static const float DancingStartTime;
//private:
//	static const uint32_t HudFramesCount = 99;
//
//	static const float StartCreditsTime;
//	static const float ReleasingTime;
//
//	bool m_switchedRobots;
//	
//	std::vector<Model*> m_models;
//
//	Texture *m_textures[3];
//
//	MechArm *m_mechArm1;
//	MechArm *m_mechArm2;
//	Robot *m_robot;
//
//	Model *m_credits;
//	PropertySignal *m_changeCredits;
//	int32_t m_currentCreditsIndex;
//	float creditsPower;
//
//	sm::Matrix m_robotWorldMatrix;
//	sm::Matrix m_dancingRobotWorldMatrix;
//
//	Texture *m_hudScreensTex[HudFramesCount];
//
//	Model *m_screens;
//	uint32_t m_screenIndex;
//	float m_screenProgress;
//
//	/*Model *m_greetz;
//	Model *m_glassWall;
//	Model *m_glassWallBroken;*/
//
//	/*Model *m_robotPos;
//	Animation *m_robotPosAnim;*/
//
//	//Texture **m_textures;
//
//	//PropertySignal *m_changeGreetz;
//
//	void DrawCredits(float time, float seconds);
//	void DrawScreen(float time, float seconds);
//	void GetLightTransform(sm::Matrix &lightTransform);
//
//	float m_currentTime;
//};
//
