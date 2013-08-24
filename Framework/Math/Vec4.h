#ifndef _VEC4_
#define _VEC4_
#pragma once

#define rad(x) (float) ((3.141592654f * x) / 180.0f)
#define deg(x) (float) ((180.0f * x) / 3.141592654f)

#include <math.h>
#include "Vec3.h"
#include <assert.h>

namespace sm
{
	class Vec4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Vec4()
		{
		}

		Vec4(float x, float y, float z, float w)
		{
			this ->x = x;
			this ->y = y;
			this ->z = z;
			this ->w = w;
		}

		Vec4(sm::Vec3 v, float w = 1.0f)
		{
			this ->x = v.x;
			this ->y = v.y;
			this ->z = v.z;
			this ->w = w;
		}

		void Set(float x, float y, float z, float w)
		{
			this ->x = x;
			this ->y = y;
			this ->z = z;
			this ->w = w;
		}

		float &operator [] (int i)
		{
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
			}

			assert(false);

			return x;
		}

		const float &operator [] (int i) const
		{
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
			}

			assert(false);

			return x;
		}

		Vec4 operator * (float s) const
		{
			Vec4 vret;

			vret.x = x * s;
			vret.y = y * s;
			vret.z = z * s;
			vret.w = w * s;

			return vret;
		}

		Vec4 operator + (const Vec4 &v) const
		{
			Vec4 vret;

			vret.x = x + v.x;
			vret.y = y + v.y;
			vret.z = z + v.z;
			vret.w = w + v.w;

			return vret;
		}

		Vec4 operator - (const Vec4 &v) const
		{
			Vec4 vret;

			vret.x = x - v.x;
			vret.y = y - v.y;
			vret.z = z - v.z;
			vret.w = z - v.w;

			return vret;
		}

		Vec4 &operator *= (float s)
		{
			x = x * s;
			y = y * s;
			z = z * s;
			w = w * s;

			return *this;
		}

		Vec4 &operator += (const Vec4 &v)
		{
			x = x + v.x;
			y = y + v.y;
			z = z + v.z;
			w = w + v.w;

			return *this;
		}

		Vec4 &operator -= (const Vec4 &v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;

			return *this;
		}

		void operator = (const float v[4])
		{
			x = v[0];
			y = v[1];
			z = v[2];
			w = v[3];
		}

		bool operator == (const Vec4 &v) const
		{
			if ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w))	return false;
			return true;
		}

		bool operator != (const Vec4 &v) const
		{
			if ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w))	return true;
			return false;
		}

		void Normalize()
		{
			float length = GetLength();

			if (length == 0.0f) return;

			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}

		Vec4 GetNormalized() const
		{
			float length = GetLength();

			if (length == 0.0f)
				return Vec4(0, 0, 0, 0);

			return Vec4(x / length, y / length, z / length, w / length);
		}

		void Reverse()
		{
			x = -x;
			y = -y;
			z = -z;
			w = -w;
		}

		Vec4 GetReversed() const
		{
			return Vec4(-x, -y, -z, -w);
		}

		void SetLength(float l)
		{
			float act_length = GetLength();
			if (act_length == 0.0f) return;

			l /= act_length;

			x *= l;
			y *= l;
			z *= l;
			w *= l;
		}

		float GetLength() const
		{
			return sqrtf(x*x + y*y + z*z + w*w);
		}

		static float Dot(const Vec4 &v1, const Vec4 &v2)
		{
			return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
		}

		static Vec4 ToVec4(const float v[4])
		{
			Vec4 vret(v[0], v[1], v[2], v[4]);
			return vret;
		}

		static Vec4 ToVec4(float x, float y, float z, float w)
		{
			Vec4 vret(x, y, z, w);
			return vret;
		}
	};
}

#endif

