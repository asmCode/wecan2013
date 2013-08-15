#pragma once

#include <Math/Vec3.h>
#include <Math/Matrix.h>
#include <Math/Plane.h>

class BoundingBox;

class Frustum
{
public:
	enum FrustumFace
	{
		FrustumFace_Front = 0,
		FrustumFace_Back,
		FrustumFace_Left,
		FrustumFace_Right,
		FrustumFace_Top,
		FrustumFace_Bottom
	};

private:
	sm::Plane faces[6];

	sm::Vec3 camPos;
	sm::Vec3 camForward;
	sm::Vec3 camRight;
	sm::Vec3 camUp;

	float nearPlane;
	float farPlane;
	float verticalFov;
	float aspect;
	float tanOfHalfVertFov;

public:
	Frustum(void);

	void SetFrustum(
		/*const sm::Vec3 &camPos,
		const sm::Vec3 &camTrg,*/
		const sm::Matrix &view,
		float near,
		float far,
		float verticalFov,
		float aspect);

	bool IsPointInside(const sm::Vec3 &point);
	bool IsSphereInside(const sm::Vec3 &center, float radius, BoundingBox &bbox);
};
