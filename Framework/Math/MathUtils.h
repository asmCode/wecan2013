#pragma once
#ifndef _MATHUTILS_
#define _MATHUTILS_

#include <windows.h>
#include <math\Vec3.h>
#include <math\Vec2.h>

class MathUtils
{
private:
	static void GetDotsAndLengths(sm::Vec3 *points, float *dots, float *lengths)
	{
		for (int i = 0; i < 4; i++)
		{
			sm::Vec3 v1 = points[(4 + (i - 1)) % 4] - points[i]; // one left
			sm::Vec3 v2 = points[(i + 1) % 4] - points[i]; // one right

			lengths[i] = v2.GetLength();

			v1.Normalize();
			v2.Normalize();

			dots[i] = sm::Vec3::Dot(v1, v2);
		}
	}

public:
	static float GetDistanceFromSegment(sm::Vec3 a, sm::Vec3 b, sm::Vec3 p)
	{
		sm::Vec3 ab = b - a;
		sm::Vec3 ap = p - a;

		sm::Vec3 abn = ab;
		abn.Normalize();

		float d = sm::Vec3::Dot(abn, ap);

		sm::Vec3 h = a + (abn * d);

		sm::Vec3 ha = a - h;
		sm::Vec3 hb = b - h;

		// TODO: do poprawki! galon pokazal lepszy sposob
		if (sm::Vec3::Dot(ha, hb) < 0.0f) // czy punkt miesci sie w odcinku?
		{
			sm::Vec3 ph = h - p;
			return ph.GetLength();
		}
		else // jesli nie, to szukamy odleglosci od krancow odcinka i zwracamy mniejsza odleglosc
		{
			sm::Vec3 bp = p - b;
			float l1 = bp.GetLength();
			float l2 = ap.GetLength();

			if (l1 < l2)
				return l1;
			else
				return l2;
		}
	}

	static bool IsSquare(sm::Vec3 *points, float dotsEpsilon, float lengthsEpsilon)
	{
		float dots[4];
		float lengths[4];

		GetDotsAndLengths(points, dots, lengths);

		float dotsSum = 0;
		float lengthSum = 0;

		for (int i = 0; i < 4; i++)
		{
			dotsSum += abs(dots[i]);
			lengthSum += lengths[i];
		}

		float lengthAvg = lengthSum / 4.0f;

		float dotWeight = (4.0f - dotsSum) / 4.0f;

		float lenWeight = (4.0f -
			(abs(lengths[0] - lengthAvg) / lengthAvg) -
			(abs(lengths[1] - lengthAvg) / lengthAvg) -
			(abs(lengths[2] - lengthAvg) / lengthAvg) -
			(abs(lengths[3] - lengthAvg) / lengthAvg)) / 4.0f;

		if ((1.0f - dotWeight) <= dotsEpsilon && (1.0f - lenWeight) <= lengthsEpsilon)
			return true;
		else
			return false;
	}

	template <typename Vec3Collection>
	static sm::Vec3 GetAvgVector(Vec3Collection vectors, int count)
	{
		sm::Vec3 avg(0, 0, 0);

		for (int i = 0; i < count; i++)
			avg += vectors[i];

		return avg *= 1.0f / (float)count;
	}

	static sm::Vec3 GetTriangleHeightVector(sm::Vec3 base, sm::Vec3 arm)
	{
		sm::Vec3 basen = base;
		basen.Normalize();

		float shift = sm::Vec3::Dot(basen, arm);

		base = basen * shift;
		return arm - base;
	}

	static float GetDistanceFromLine(sm::Vec3 a, sm::Vec3 b, sm::Vec3 p)
	{
		sm::Vec3 ab = b - a;
		sm::Vec3 ap = p - a;

		sm::Vec3 abn = ab;
		abn.Normalize();

		float d = sm::Vec3::Dot(abn, ap);

		sm::Vec3 h = a + (abn * d);

		sm::Vec3 ph = h - p;
		return ph.GetLength();
	}

	static bool IsPointInTriangle(sm::Vec3 a, sm::Vec3 b, sm::Vec3 c, sm::Vec3 p)
	{
		sm::Vec3 ab = b - a;
		sm::Vec3 ac = c - a;
		sm::Vec3 ap = p - a;

		sm::Vec3 ba = a - b;
		sm::Vec3 bc = c - b;
		sm::Vec3 bp = p - b;

		sm::Vec3 ca = a - c;
		sm::Vec3 cb = b - c;
		sm::Vec3 cp = p - c;

		return
			sm::Vec3::Dot(ab * ac, ab * ap) >= 0 &&
			sm::Vec3::Dot(ac * ab, ac * ap) >= 0 &&
			sm::Vec3::Dot(bc * ba, bc * bp) >= 0;
	}

	static bool IsPointInTriangle(sm::Vec2 a, sm::Vec2 b, sm::Vec2 c, sm::Vec2 p)
	{
		return IsPointInTriangle(
			sm::Vec3(a.x, a.y, 0), sm::Vec3(b.x, b.y, 0), sm::Vec3(c.x, c.y, 0), sm::Vec3(p.x, p.y, 0));
	}

	template <typename Vec3Collection>
	static bool IsPointInConvexSet(sm::Vec3 point, Vec3Collection convexSet, int convexSetPointsCount)
	{
		if (convexSetPointsCount < 3)
			return false;

		float angle = 0.0f;

		for (int i = 0; i < convexSetPointsCount; i++)
		{
			angle += sm::Vec3::GetAngle(
				(convexSet[i] - point).GetNormalized(),
				(convexSet[(i + 1) % convexSetPointsCount] - point).GetNormalized());
		}

		return angle > 6.2f;
	}

	template <typename Vec3Collection>
	static float GetConvexSetArea(Vec3Collection convexSet, int convexSetPointsCount)
	{
		if (convexSetPointsCount < 3)
			return 0.0f;

		float area = 0.0f;
		sm::Vec3 point = GetAvgVector(convexSet, convexSetPointsCount);

		for (int i = 0; i < convexSetPointsCount; i++)
			area += GetTriangleArea(point, convexSet[i], convexSet[(i + 1) % convexSetPointsCount]);

		return area;
	}

	static float GetTriangleArea(sm::Vec3 a, sm::Vec3 c, sm::Vec3 b)
	{
		return ((c - a) * (b - a)).GetLength() / 2.0f;
	}

	static float GetQuadArea(sm::Vec3 a, sm::Vec3 b, sm::Vec3 c, sm::Vec3 d)
	{
		return GetTriangleArea(a, b, c) + GetTriangleArea(c, d, a);
	}

	template <typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template <typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	static float PI;
	static float PI2;
	static float PI4;
};

#endif
