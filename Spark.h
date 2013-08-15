#pragma once

#include <Math\Vec3.h>

class Spark
{
public:
	Spark();

	void Start(const sm::Vec3 &position);
	
	sm::Vec3 m_position;
	sm::Vec3 m_moveTarget;
	float m_liteTime;
	bool m_isUsed;
};

