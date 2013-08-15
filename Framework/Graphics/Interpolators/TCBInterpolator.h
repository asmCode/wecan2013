#pragma once

#include "Interpolator.h"
#include "KeyFrame.h"
#include <vector>

template <typename Type>
class TCBInterpolator : public Interpolator<Type>
{
private:
	//catmull-rom splines
	int GetVectors(float time, float &normalizedTime, Type &pt1, Type &pt2, Type &tn1, Type &tn2, int lastKeyframeIndex)
	{
		//key on right side
		std::vector<KeyFrame<Type>*>::iterator rk;

		for (rk = keys.begin() + lastKeyframeIndex; rk != keys.end(); rk++, lastKeyframeIndex++)
			if ((*rk) ->time >= time)
				break;

		//time normalization
		normalizedTime = (time - (*(rk - 1)) ->time) / ((*rk) ->time - (*(rk - 1)) ->time);

		pt1 = (*(rk - 1)) ->value;
		pt2 = (*rk) ->value;

		if ((rk - 1) != keys.begin() && !(*(rk - 2)) ->stopKey)
			tn1 = (Type)((pt2 - (*(rk - 2)) ->value) * 0.5f);
		else
			tn1 = (Type)((pt2 - pt1) * 1.5f);

		if ((rk + 1) != keys.end() && !(*rk) ->stopKey)
			tn2 = (Type)(((*(rk + 1)) ->value - pt1) * 0.5f);
		else
			tn2 = (Type)(((*rk) ->value - (*(rk - 1)) ->value) * 1.5f);

		if (keys.size() == 2)
			tn1 = tn2 = pt2 - pt1;
		else
		{
			if ((rk - 1) == keys.begin())
				tn1 = (Type)(tn1 - (tn2 * 0.5f));

			if ((rk + 1) == keys.end())
				tn2 = (Type)(tn2 - (tn1 * 0.5f));
		}

		return lastKeyframeIndex;
	}

public:
	int GetValue(float time, Type &value, int lastKeyframeIndex = 0)
	{
		if (keys.size() == 0)
			return 0;

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

		// check if stop key
		//key on right side
		std::vector<KeyFrame<Type>*>::iterator rk;

		for (rk = keys.begin() + lastKeyframeIndex; rk != keys.end(); rk++, lastKeyframeIndex++)
			if ((*rk) ->time >= time)
				break;

		if ((*rk) ->time == time)
		{
			value = (*rk) ->value;
			return lastKeyframeIndex;
		}

		if ((*(rk - 1)) ->stopKey)
		{
			value = (*(rk - 1)) ->value;
			return lastKeyframeIndex;
		}
		//

		Type pt1;
		Type pt2;
		Type tn1;
		Type tn2;
		float normTime;

		GetVectors(time, normTime, pt1, pt2, tn1, tn2, lastKeyframeIndex);

		float t1 = normTime;
		float t2 = t1 * normTime;
		float t3 = t2 * normTime;

		value =
			(Type)
			(
			(pt1 * (2.0f * t3 - 3.0f * t2 + 1)) +
			(pt2 * (-2.0f * t3 + 3.0f * t2)) +
			(tn1 * (t3 - 2.0f * t2 + t1)) +
			(tn2 * (t3 - t2))
			);

		return lastKeyframeIndex;
	}
};

