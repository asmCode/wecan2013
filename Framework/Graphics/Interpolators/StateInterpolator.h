#pragma once

#include "Interpolator.h"
#include "KeyFrame.h"
#include <vector>

template <typename Type>
class StateInterpolator : public Interpolator<Type>
{
public:
	int GetValue(float time, Type &value, int lastKeyframeIndex = 0)
	{
		if (keys.size() == 0)
		{
			//value = defaultValue;
			return 0;
		}

		if (time <= (*keys.begin()) ->time)
		{
			value = (*keys.begin()) ->value;
			return 0;
		}

		if (time >= (*(keys.end() - 1)) ->time)
		{
			value = (*(keys.end() - 1)) ->value;
			return keys.size() - 1;
		}

		//key on right side
		std::vector<KeyFrame<Type>*>::iterator rk;

		for (rk = keys.begin() + lastKeyframeIndex; rk != keys.end(); rk++)
		{
			if ((*rk) ->time > time)
				break;

			lastKeyframeIndex++;
		}

		value = (*(rk - 1)) ->value;

		return lastKeyframeIndex - 1;
	}
};
