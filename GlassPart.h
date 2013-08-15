#pragma once

#include <Math\Matrix.h>
#include <Math\Vec3.h>

class Mesh;

class GlassPart
{
public:
	Mesh *m_mesh;

	sm::Vec3 m_position;
	sm::Vec3 m_moveTarget;
	sm::Vec3 m_rotateAxis;
	float m_rotateAngle;
	float m_rotateSpeed;
};

