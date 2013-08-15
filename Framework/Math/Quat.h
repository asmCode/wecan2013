#pragma once

#include <math.h>

#include "Vec3.h"

namespace sm
{
	class Quat
	{
	private:
	public:
		float s;
		Vec3 v;

		Quat()
		{
		}

		Quat(float s, float x, float y, float z)
		{
			this ->v.Set(x, y, z);
			this ->s = s;
		}

		void Set(float s, float x, float y, float z)
		{
			this ->v.Set(x, y, z);
			this ->s = s;
		}

		Quat operator + (Quat q)
		{
			Quat ret;
			ret.v = this ->v + q.v;
			ret.s = this ->s + q.s;
			return ret;
		}

		Quat operator - (Quat q)
		{
			Quat ret;
			ret.v = this ->v - q.v;
			ret.s = this ->s - q.s;
			return ret;
		}

		Quat operator * (Quat q)
		{
			Quat ret;
			Vec3 v1, v2, v3;
			v1 = v * q.s;
			v2 = q.v * s;
			v3 = v * q.v;
			ret.v = v1 + v2 + v3;
			ret.s = (s * q.s) - Vec3::Dot(v, q.v);
			return ret;
		}

		Quat operator * (float s)
		{
			Quat ret;
			ret.v = this ->v * s;
			ret.s = this ->s * s;
			return ret;
		}

		Quat GetCoupleQuat()
		{
			Quat ret;
			ret.v = v;
			ret.v.Reverse();
			ret.s = s;
			return ret;
		}

		void RotateToQuat(float a, float x, float y, float z)
		{
			v.Set(x, y, z);
			v = v * sinf(a/2.0f);
			s = cosf(a/2.0f);
			Normalize();
		}

		void RotateToQuat(float a, Vec3 axis)
		{
			v = axis;
			v = v * sinf(a / 2.0f);
			s = cosf(a / 2.0f);
			Normalize();
		}

		void QuatToRotate(float &a, Vec3 &v)
		{
			a = acosf(this ->s) * 2.0f;

			//HMMMMMM;)
			if (a == 0) return;

			v = this ->v * (1.0f / sinf(a / 2.0f));
		}

		void Normalize()
		{
			float len = GetLength();

			if (len != 0.0f)
			{
				float proportion = 1.0f / len;
				v = v * proportion;
				s = s * proportion;
			}
		}

		float GetLength()
		{
			return sqrtf((s * s) + Vec3::Dot(v, v));
		}

		Vec3 Rotate(Vec3 v)
		{
			Quat ret;
			Quat coup = GetCoupleQuat();

			ret.v = v;
			ret.s = 0.0f;
			ret = (*this) * ret;
			ret = ret * coup;

			return ret.v;
		}
	};
}
