#pragma once

#include "Interpolator.h"
#include "KeyFrame.h"
#include <vector>

template <typename Type>
class StateInterpolator : public Interpolator<Type>
{
public:
	void GetValue(float time, Type &value)
	{
		if (keys.size() == 0)
		{
			//value = defaultValue;
			return;
		}

		if (time <= (*keys.begin()) ->time)
		{
			value = (*keys.begin()) ->value;
			return;
		}

		if (time >= (*(keys.end() - 1)) ->time)
		{
			value = (*(keys.end() - 1)) ->value;
			return;
		}

		//key on right side
		std::vector<KeyFrame<Type>*>::iterator rk;

		for (rk = keys.begin(); rk != keys.end(); rk++)
			if ((*rk) ->time > time)
				break;

		value = (*(rk - 1)) ->value;
	}
};
