#pragma once

#include "../GameObject.h"
#include <Graphics/Property.h>
#include <vector>

class Model;
class Mesh;
class Animation;
class Shader;
class Texture;

class CreditsDance : public GameObject
{
public:
	~CreditsDance();

	void Awake();

	void Update(float time, float seconds);
	void Draw();

	void DrawOpacities();

	std::vector<MeshPart*>& GetMeshParts();

	float GetAnimTime() const;
	bool IsActive() const;

private:
	Animation *m_creditsHandAnim;
	Animation *m_handNodeAnim;
	Model *m_creditsHand;
	Mesh *m_handTrg;
	Mesh *m_handAxis;
	Mesh *m_handUp;
	Property *m_creditsVisible;
	Shader *m_creditSpriteShader;
	Texture **m_greetzTexes;

	sm::Vec3 m_startPos;
	sm::Vec3 m_dir;
	sm::Vec3 m_up;
	float m_creditTexWidth;

	std::vector<MeshPart*> m_allMeshParts;

	float m_danceTimeAddRatio;

	bool m_creditsActive;
	float m_creditsStartTime;
	float m_creditsLength;
	float m_creditsProgress;

	// separate time to take time lapse under account
	float m_danceTime;

	bool m_d;

	int m_creditsIndex;

	float m_baseTime;
};

