#ifndef _PLANE_
#define _PLANE_

#pragma once

#include "Vec3.h"

namespace sm
{
	class Plane
	{
	public:
		Vec3 normal;
		Vec3 point;
		float d;

	Plane::Plane()
	{
		this ->normal.Set(0, 0, 1);
		this ->point.Set(0, 0, 0);
		d = -sm::Vec3::Dot(normal, point);
	}

	Plane::Plane(Vec3 normal, Vec3 point)
	{
		this ->normal = normal;
		this ->point = point;
		d = -sm::Vec3::Dot(normal, point);
	}

	void Plane::Set(Vec3 normal, Vec3 point)
	{
		this ->normal = normal;
		this ->point = point;
		d = -sm::Vec3::Dot(normal, point);
	}

	bool Plane::GetIntersection(Vec3 target, Vec3 point, Vec3 &intersectionPoint)
	{
		float denominator = sm::Vec3::Dot(normal, target);

		if (denominator == 0)
			return false;

		float t = -(sm::Vec3::Dot(normal, point) + d) /
			denominator;

		intersectionPoint = point + (target * t);

		return true;
	}
	};
}

#endif