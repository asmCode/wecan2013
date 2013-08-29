#include "Frustum.h"
#include <Graphics/BoundingBox.h>

Frustum::Frustum(void)
{
}

void Frustum::SetFrustum(
	const sm::Matrix &view,
	float nearPlane,
	float farPlane,
	float verticalFov,
	float aspect)
{
	sm::Matrix viewInv = view.GetInversed();

	this->camPos = sm::Vec3(viewInv.a[12], viewInv.a[13], viewInv.a[14]);
	this->camForward = sm::Vec3(-viewInv.a[8], -viewInv.a[9], -viewInv.a[10]);
	this->camRight = sm::Vec3(viewInv.a[0], viewInv.a[1], viewInv.a[2]);
	this->camUp = sm::Vec3(viewInv.a[4], viewInv.a[5], viewInv.a[6]).GetNormalized();
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	this->verticalFov = verticalFov;
	this->aspect = aspect;

	this->tanOfHalfVertFov = tanf(this->verticalFov / 2);
}

bool Frustum::IsPointInside(const sm::Vec3 &point)
{
	return true;
}

bool Frustum::IsSphereInside(const sm::Vec3 &center2, float radius, BoundingBox &bbox)
{
	static sm::Vec3 pointTrg[8];
	static float x[8];
	static float y[8];
	static float z[8];
	for (char i = 0; i < 8; i++)
	{
		pointTrg[i] = bbox.GetBBoxVerticle(i) - camPos;
		z[i] = sm::Vec3::Dot(camForward, pointTrg[i]);
	}

	bool outside = true;
	for (unsigned i = 0; i < 8; i++)
	{
		if (z[i] > nearPlane)
		{
			outside = false;
			break;
		}
	}
	if (outside)
		return false;

	outside = true;
	for (unsigned i = 0; i < 8; i++)
	{
		if (z[i] < farPlane)
		{
			outside = false;
			break;
		}
	}
	if (outside)
		return false;

	for (char i = 0; i < 8; i++)
	{
		y[i] = sm::Vec3::Dot(camUp, pointTrg[i]);
	}

	outside = true;
	for (unsigned i = 0; i < 8; i++)
	{
		float h = z[i] * tanOfHalfVertFov;

		if (y[i] < h)
		{
			outside = false;
			break;
		}
	}
	if (outside)
		return false;

	outside = true;
	for (unsigned i = 0; i < 8; i++)
	{
		float h = z[i] * tanOfHalfVertFov;

		if (y[i] > -h)
		{
			outside = false;
			break;
		}
	}
	if (outside)
		return false;

	for (char i = 0; i < 8; i++)
	{
		x[i] = sm::Vec3::Dot(camRight, pointTrg[i]);
	}

	outside = true;
	for (unsigned i = 0; i < 8; i++)
	{
		float h = z[i] * tanOfHalfVertFov;
		float w = h * aspect;

		if (x[i] < w)
		{
			outside = false;
			break;
		}
	}
	if (outside)
		return false;

	outside = true;
	for (unsigned i = 0; i < 8; i++)
	{
		float h = z[i] * tanOfHalfVertFov;
		float w = h * aspect;

		if (x[i] > -w)
		{
			outside = false;
			break;
		}
	}
	if (outside)
		return false;

	return true;
}
