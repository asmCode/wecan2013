#pragma once

namespace sm
{
	template <typename T>
	class Point
	{
	public:
		Point()
		{
		}
		
		Point(T x, T y)
		{
			this ->X = x;
			this ->Y = y;
		}
		
		T X;
		T Y;
	};
}
