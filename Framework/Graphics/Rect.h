#pragma once

namespace sm
{
	template <typename T>
	class Rect
	{
	public:
		Rect()
		{
			this ->X = 0;
			this ->Y = 0;
			this ->Width = 0;
			this ->Height = 0;
		}
		
		Rect(T x, T y, T width, T height)
		{
			this ->X = x;
			this ->Y = y;
			this ->Width = width;
			this ->Height = height;
		}
		
		T X;
		T Y;
		T Width;
		T Height;
	};
}