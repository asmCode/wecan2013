#ifndef _MATRIX_
#define _MATRIX_
#pragma once

#include "Vec3.h"
#include "Vec4.h"

//[0] [4] [8]  [12]     [0]
//[1] [5] [9]  [13]     [1]
//[2] [6] [10] [14]     [2]
//[3] [7] [11] [15]     [3]

namespace sm
{
	class Matrix
	{
	public:
		float a[16];

		static Matrix IdentityMatrix()
		{
			Matrix matrix;
			
			for (int i = 0; i < 16; i++)
				if (i % 5 == 0)
					matrix.a[i] = 1.0f;
				else
					matrix.a[i] = 0.0f;
			
			
			return matrix;
		}
		
		Matrix GetTransposed() const
		{
			Matrix matrix;
			for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					matrix.a[y * 4 + x] = a[x * 4 + y];
			
			return matrix;
		}
				
		Matrix GetInversed() const
		{
			Matrix src = this ->GetTransposed();
			Matrix dst;
			float tmp[12];
			float det;
			
			tmp[0] = src[10] * src[15];
			tmp[1] = src[11] * src[14];
			tmp[2] = src[9] * src[15];
			tmp[3] = src[11] * src[13];
			tmp[4] = src[9] * src[14];
			tmp[5] = src[10] * src[13];
			tmp[6] = src[8] * src[15];
			tmp[7] = src[11] * src[12];
			tmp[8] = src[8] * src[14];
			tmp[9] = src[10] * src[12];
			tmp[10] = src[8] * src[13];
			tmp[11] = src[9] * src[12];
			
			dst.a[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
			dst.a[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
			dst.a[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
			dst.a[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
			dst.a[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
			dst.a[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
			dst.a[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
			dst.a[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
			dst.a[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
			dst.a[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
			dst.a[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
			dst.a[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
			dst.a[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
			dst.a[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
			dst.a[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
			dst.a[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
			
			tmp[0] = src[2]*src[7];
			tmp[1] = src[3]*src[6];
			tmp[2] = src[1]*src[7];
			tmp[3] = src[3]*src[5];
			tmp[4] = src[1]*src[6];
			tmp[5] = src[2]*src[5];
			tmp[6] = src[0]*src[7];
			tmp[7] = src[3]*src[4];
			tmp[8] = src[0]*src[6];
			tmp[9] = src[2]*src[4];
			tmp[10] = src[0]*src[5];
			tmp[11] = src[1]*src[4];
			
			dst.a[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
			dst.a[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
			dst.a[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
			dst.a[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
			dst.a[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
			dst.a[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
			dst.a[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
			dst.a[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
			dst.a[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
			dst.a[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
			dst.a[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
			dst.a[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
			dst.a[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
			dst.a[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
			dst.a[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
			dst.a[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
			
			det=src[0]*dst.a[0]+src[1]*dst.a[1]+src[2]*dst.a[2]+src[3]*dst.a[3];
			
			det = 1.0f / det;
			for (int j = 0; j < 16; j++)
				dst.a[j] *= det;
			
			return dst;
		}
		
		static Matrix TranslateMatrix(const Vec3 &v)
		{
			return TranslateMatrix(v.x, v.y, v.z);
		}

		static Matrix TranslateMatrix(float x, float y, float z)
		{
			Matrix matrix = IdentityMatrix();

			matrix.a[12] = x;
			matrix.a[13] = y;
			matrix.a[14] = z;

			return matrix;
		}

		static Matrix ScaleMatrix(const Vec3 &v)
		{
			return ScaleMatrix(v.x, v.y, v.z);
		}

		static Matrix ScaleMatrix(float x, float y, float z)
		{
			Matrix matrix = IdentityMatrix();

			matrix.a[0] = x;
			matrix.a[5] = y;
			matrix.a[10] = z;

			return matrix;
		}

		//TODO: nietestowane!
		static Matrix RotateXMatrix(float angle)
		{
			Matrix matrix = IdentityMatrix();

//			float tsin = sinf(angle);
	//		float tcos = cosf(angle);

			/*matrix.a[1][1] = tcos;
			matrix.a[1][2] = tsin;
			matrix.a[2][1] = -tsin;
			matrix.a[2][2] = tcos;*/

			return matrix;
		}

		static Matrix RotateYMatrix(float angle)
		{
			Matrix matrix = IdentityMatrix();
//
		//	float tsin = sinf(angle);
//float tcos = cosf(angle);

			/*matrix.a[0][0] = tcos;
			matrix.a[0][2] = -tsin;
			matrix.a[2][0] = tsin;
			matrix.a[2][2] = tcos;*/

			return matrix;
		}

		static Matrix RotateZMatrix(float angle)
		{
			Matrix matrix = IdentityMatrix();

		//	float tsin = sinf(angle);
		//	float tcos = cosf(angle);

			/*matrix.a[1][0] = tcos;
			matrix.a[1][1] = tsin;
			matrix.a[2][0] = -tsin;
			matrix.a[2][1] = tcos;*/

			return matrix;
		}

		static Matrix RotateAxisMatrix(float angle, const Vec3 &axis)
		{
			return RotateAxisMatrix(angle, axis.x, axis.y, axis.z);
		}

		static Matrix RotateAxisMatrix(float angle, float x, float y, float z)
		{
			Matrix matrix = IdentityMatrix();

			float c = cosf(angle);
			float s = sinf(angle);

			float oneMinusC = (1 - c);

			matrix.a[0] = x * x * oneMinusC + c;
			matrix.a[1] = y * x * oneMinusC + z * s;
			matrix.a[2] = x * z * oneMinusC - y * s;
			matrix.a[4] = x * y * oneMinusC - z * s;
			matrix.a[5] = y * y * oneMinusC + c;
			matrix.a[6] = y * z * oneMinusC + x * s;
			matrix.a[8] = x * z * oneMinusC + y * s;
			matrix.a[9] = y * z * oneMinusC - x * s;
			matrix.a[10] = z * z * oneMinusC + c;

			return matrix;
		}

		static Matrix PerspectiveMatrix(float fov, float aspect, float znear, float zfar)
		{
			fov = 3.141592653f * (fov / 180.0f);

			Matrix matrix = IdentityMatrix();

			float f = 1.0f / tanf(fov / 2.0f);
			float nearMinusFar = znear - zfar;

			matrix.a[0] = f / aspect;
			matrix.a[5] = f;
			matrix.a[10] = (zfar + znear) / nearMinusFar;
			matrix.a[11] = -1;
			matrix.a[14] = (2.0f * znear * zfar) / nearMinusFar;
			matrix.a[15] = 0.0f;

			return matrix;
		}

		static Matrix Ortho2DMatrix(float left, float right, float bottom, float top)
		{
			Matrix matrix = IdentityMatrix();

			matrix.a[0] = 2.0f / (right - left);
			matrix.a[5] = 2.0f / (top - bottom);
			matrix.a[10] = -1.0f;

			matrix.a[12] = -(right + left) / (right - left);
			matrix.a[13] = -(top + bottom) / (top - bottom);

			return matrix;
		}

		bool operator == (const Matrix &matrix) const
		{
			for (int i = 0; i < 16; i++)
				if (a[i] != matrix.a[i])
					return false;

			return true;
		}

		Matrix operator * (const Matrix &right) const
		{
			Matrix left;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					float val = 0.0f;
					for (int k = 0; k < 4; k++)
						val += a[(k * 4) + i] * right.a[(j * 4) + k];
					left.a[(j * 4) + i] = val;
				}
			}

			return left;
		}

		Vec3 operator * (const Vec3 &right) const
		{
			Vec4 right4(right);
			Vec4 left;

			for (int i = 0; i < 4; i++)
			{
				float val = 0.0f;
				for (int k = 0; k < 4; k++)
					val += a[(k * 4) + i] * right4[k];
				left[i] = val;
			}

			return Vec3(left.x, left.y, left.z);
		}

		Matrix &operator *= (const Matrix &right)
		{
			(*this) = (*this) * right;
			return *this;
		}

		void operator =(const float *a)
		{
			for (int i = 0; i < 16; i++)
				 this ->a[i] = a[i];
		}

		operator float*()
		{
			return a;
		}

		operator const float*() const
		{
			return a;
		}

		void SetValue(int row, int column, float value)
		{
			a[(column * 4) + row] = value;
		}

		Vec3 ExtractScale() const
		{
			Vec3 xAxis(a[0], a[1], a[2]);
			Vec3 yAxis(a[4], a[5], a[6]);
			Vec3 zAxis(a[8], a[9], a[10]);

			/*Vec3 xAxis(a[0], a[4], a[8]);
			Vec3 yAxis(a[1], a[5], a[9]);
			Vec3 zAxis(a[2], a[6], a[10]);*/

			return Vec3(xAxis.GetLength(), yAxis.GetLength(), zAxis.GetLength());
			//return Vec3(1, 1, 1);
		}

		static Matrix CreateLookAt(const sm::Vec3 &direction, const sm::Vec3 &up)
		{
			sm::Matrix rot = sm::Matrix::IdentityMatrix();

			sm::Vec3 right = (direction * up).GetNormalized();
			sm::Vec3 yAxis = right * direction;

			rot.a[0] = right.x;
			rot.a[1] = right.y;
			rot.a[2] = right.z;

			rot.a[4] = up.x;
			rot.a[5] = up.y;
			rot.a[6] = up.z;

			rot.a[8] = direction.x;
			rot.a[9] = direction.y;
			rot.a[10] = direction.z;

			return rot;
		}
	};
}

#endif