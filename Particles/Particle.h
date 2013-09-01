#pragma once

#include <Math\Vec3.h>
#include <Math\Vec4.h>

class Particle
{
public:
	Particle();
	virtual ~Particle() {};

	bool m_isUsed;

	float m_size;
	sm::Vec3 m_position;
	sm::Vec4 m_color;
	sm::Vec3 m_moveDirection;

	
	float m_liteTimeLimit;
	float m_liteTime;
};

