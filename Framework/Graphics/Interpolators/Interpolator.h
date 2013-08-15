#pragma once

#include <windows.h>

#include <vector>
#include "KeyFrame.h"
#include "IInterpolator.h"

template <class Type>
class Interpolator : public IInterpolator<Type>
{
protected:
	std::vector<KeyFrame<Type>*> keys;

public:
	void AddKeyframe(float time, Type value, bool stopKey)
	{
		KeyFrame<Type> *key = new KeyFrame<Type>();
		key ->time = time;
		key ->value = value;
		key ->stopKey = stopKey;

		keys.push_back(key);
	}

	void InsertKeyframe(float time, Type value, bool stopKey)
	{
		KeyFrame<Type> *key = new KeyFrame<Type>();
		key ->time = time;
		key ->value = value;
		key ->stopKey = stopKey;

		std::vector<KeyFrame<Type>*>::iterator i;
		for (i = keys.begin(); i != keys.end(); i++)
			if ((*i) ->time >= key ->time)
				break;

		if (i != keys.end() && (*i) ->time == time)
		{
			(*i) ->value = value;
			(*i) ->stopKey = stopKey;
		}
		else
			keys.insert(i, key);
	}

	void GetKeyframe(int index, float &time, Type &value, bool &stopKey)
	{
		KeyFrame<Type> *key = keys[index];
		time = key ->time;
		value = key ->value;
		stopKey = key ->stopKey;
	}

	int GetKeysCount()
	{
		return static_cast<int>(keys.size());
	}

	void DeleteKey(int index)
	{
		keys.erase(keys.begin() + index);
	}
};
