#ifndef _VEC3_
#define _VEC3_
#pragma once

#define rad(x) (float) ((3.141592654f * x) / 180.0f)
#define deg(x) (float) ((180.0f * x) / 3.141592654f)

#include <math.h>
#include <assert.h>

namespace sm
{
	class Vec3
	{
	public:
		float x;
		float y;
		float z;

		Vec3()
		{
		}

		Vec3(float x, float y, float z)
		{
			this ->x = x;
			this ->y = y;
			this ->z = z;
		}

		void Set(float x, float y, float z)
		{
			this ->x = x;
			this ->y = y;
			this ->z = z;
		}

		float &operator [] (int i)
		{
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
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
			}

			assert(false);

			return x;
		}

		Vec3 operator * (const Vec3 &v) const
		{
			Vec3 vret;

			vret.x = (v.y * z) - (y * v.z);
			vret.y = (x * v.z) - (v.x * z);
			vret.z = (v.x * y) - (x * v.y);

			return vret;
		}

		Vec3 operator * (float s) const
		{
			Vec3 vret;

			vret.x = x * s;
			vret.y = y * s;
			vret.z = z * s;

			return vret;
		}

		Vec3 operator + (const Vec3 &v) const
		{
			Vec3 vret;

			vret.x = x + v.x;
			vret.y = y + v.y;
			vret.z = z + v.z;

			return vret;
		}

		Vec3 operator - (const Vec3 &v) const
		{
			Vec3 vret;

			vret.x = x - v.x;
			vret.y = y - v.y;
			vret.z = z - v.z;

			return vret;
		}

		Vec3 &operator *= (const Vec3 &v)
		{
			x = (v.y * z) - (y * v.z);
			y = (x * v.z) - (v.x * z);
			z = (v.x * y) - (x * v.y);

			return *this;
		}

		Vec3 &operator *= (float s)
		{
			x = x * s;
			y = y * s;
			z = z * s;

			return *this;
		}

		Vec3 &operator += (const Vec3 &v)
		{
			x = x + v.x;
			y = y + v.y;
			z = z + v.z;

			return *this;
		}

		Vec3 &operator -= (const Vec3 &v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;

			return *this;
		}

		void operator = (const float v[3])
		{
			x = v[0];
			y = v[1];
			z = v[2];
		}

		bool operator == (const Vec3 &v) const
		{
			if ((x != v.x) || (y != v.y) || (z != v.z))	return false;
			return true;
		}

		bool operator != (const Vec3 &v) const
		{
			if ((x != v.x) || (y != v.y) || (z != v.z))	return true;
			return false;
		}

		void Normalize()
		{
			float length = GetLength();

			if (length == 0.0f) return;

			x /= length;
			y /= length;
			z /= length;
		}

		Vec3 GetNormalized() const
		{
			float length = GetLength();

			if (length == 0.0f)
				return Vec3(0, 0, 0);

			return Vec3(x / length, y / length, z / length);
		}

		void Reverse()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		Vec3 GetReversed() const
		{
			return Vec3(-x, -y, -z);
		}

		void SetLength(float l)
		{
			float act_length = GetLength();
			if (act_length == 0.0f) return;

			l /= act_length;

			x *= l;
			y *= l;
			z *= l;
		}

		float GetLength() const
		{
			return sqrtf(x*x + y*y + z*z);
		}

		void RotateX(float angle)
		{
			float	tsin = sinf(angle);
			float	tcos = cosf(angle);
			float	ty = y;
			float	tz = z;

			y = ty * tcos - tz * tsin;
			z = ty * tsin + tz * tcos;
		}

		void RotateY(float angle)
		{
			float	tsin = sinf(angle);
			float	tcos = cosf(angle);
			float	tx = x;
			float	tz = z;

			x = tx * tcos - tz * tsin;
			z = tx * tsin + tz * tcos;
		}

		void RotateZ(float angle)
		{
			float	tsin = sinf(angle);
			float	tcos = cosf(angle);
			float	tx = x;
			float	ty = y;

			x = tx * tcos - ty * tsin;
			y = tx * tsin + ty * tcos;
		}

		static float Dot(const Vec3 &v1, const Vec3 &v2)
		{
			return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
		}

		static float GetAngle(const Vec3 &v1, const Vec3 &v2)
		{
			float angle;

			angle = Dot(v1, v2);
			if (angle > 1.0f) angle = 1.0f;
			if (angle < -1.0f) angle = -1.0f;

			angle = acosf(angle);

			return angle;
		}

		static Vec3 ToVec3(const float v[3])
		{
			Vec3 vret(v[0], v[1], v[2]);
			return vret;
		}

		static Vec3 ToVec3(float x, float y, float z)
		{
			Vec3 vret(x, y, z);
			return vret;
		}

		static Vec3 Reflect(const sm::Vec3 &normal, const sm::Vec3 &ray)
		{
			return ray - (normal * Dot(normal, ray) * 2.0f);
		}
	};
}

#endif

