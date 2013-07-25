#pragma once

#include "Interpolator.h"
#include "KeyFrame.h"
#include <vector>

template <typename Type>
class LinearInterpolator : public Interpolator<Type>
{
public:
	void GetValue(float time, Type &value)
	{
		int keysCount = keys.size();

		if (keysCount == 0)
			return;

		// if before or on first key
		if (time <= keys[0]->time)
		{
			value = keys[0]->value;
			return;
		}

		// if after or on last key
		if (time >= keys[keysCount - 1]->time)
		{
			value = keys[keysCount - 1]->value;
			return;
		}

		// if between first and last key
		int i = 0;
		for (i = 0; i < keysCount; i++)
		{
			if (keys[i]->time == time)
			{
				value = keys[i]->value;
				return;
			}

			if (keys[i]->time > time)
				break;
		}

		float normalizedTime = (time - keys[i - 1]->time) / (keys[i]->time - keys[i - 1]->time);
		value = (Type)((Type)keys[i - 1]->value * (1.0f - normalizedTime) + (Type)keys[i]->value * normalizedTime);
	}
};

