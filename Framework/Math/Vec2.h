#ifndef _VEC2_
#define _VEC2_
#pragma once

#define rad(x) (float) ((3.141592654f * x) / 180.0f)
#define deg(x) (float) ((180.0f * x) / 3.141592654f)

#include <math.h>
#include <assert.h>

namespace sm
{
	class Vec2
	{
	public:
		float x;
		float y;

		Vec2()
		{
		}

		Vec2(float x, float y)
		{
			this ->x = x;
			this ->y = y;
		}

		void Set(float x, float y)
		{
			this ->x = x;
			this ->y = y;
		}

		float &operator [] (int i)
		{
			switch (i)
			{
			case 0: return x;
			case 1: return y;
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
			}

			assert(false);

			return x;
		}

		Vec2 operator * (float s) const
		{
			Vec2 vret;

			vret.x = x * s;
			vret.y = y * s;

			return vret;
		}

		Vec2 operator + (const Vec2 &v) const
		{
			Vec2 vret;

			vret.x = x + v.x;
			vret.y = y + v.y;

			return vret;
		}

		Vec2 operator - (const Vec2 &v) const
		{
			Vec2 vret;

			vret.x = x - v.x;
			vret.y = y - v.y;

			return vret;
		}

		Vec2 &operator *= (float s)
		{
			x = x * s;
			y = y * s;

			return *this;
		}

		Vec2 &operator += (const Vec2 &v)
		{
			x = x + v.x;
			y = y + v.y;

			return *this;
		}

		Vec2 &operator -= (const Vec2 &v)
		{
			x -= v.x;
			y -= v.y;

			return *this;
		}

		void operator = (const float v[2])
		{
			x = v[0];
			y = v[1];
		}

		bool operator ==(const Vec2 &v) const
		{
			if ((x != v.x) || (y != v.y)) return false;
			return true;
		}

		bool operator !=(const Vec2 &v) const
		{
			if ((x != v.x) || (y != v.y)) return true;
			return false;
		}

		void Normalize()
		{
			float length = GetLength();

			if (length == 0.0f) return;

			x /= length;
			y /= length;
		}

		Vec2 GetNormalized() const
		{
			float length = GetLength();

			if (length == 0.0f) return Vec2(0, 0);

			return Vec2(x / length, y / length);
		}

		void Reverse()
		{
			x = -x;
			y = -y;
		}

		Vec2 GetReversed() const
		{
			return Vec2(-x, -y);
		}

		void SetLength(float l)
		{
			float act_length = GetLength();
			if (act_length == 0.0f) return;

			l /= act_length;

			x *= l;
			y *= l;
		}

		float GetLength() const
		{
			return sqrtf(x * x + y * y);
		}

		void Rotate(float angle)
		{
			float	tsin = sinf(angle);
			float	tcos = cosf(angle);
			float	tx = x;
			float	ty = y;

			x = tx * tcos - ty * tsin;
			y = tx * tsin + ty * tcos;
		}

		static float Dot(Vec2 &v1, Vec2 &v2)
		{
			return (v1.x * v2.x) + (v1.y * v2.y);
		}

		static float GetAngle(Vec2 &v1, Vec2 &v2)
		{
			float angle;

			angle = Dot(v1, v2);
			if (angle > 1.0f) angle = 1.0f;
			if (angle < -1.0f) angle = -1.0f;

			angle = acosf(angle);

			return angle;
		}

		static Vec2 ToVec2(const float v[2])
		{
			Vec2 vret(v[0], v[1]);
			return vret;
		}

		static Vec2 ToVec2(float x, float y)
		{
			Vec2 vret(x, y);
			return vret;
		}
	};
}

#endif

