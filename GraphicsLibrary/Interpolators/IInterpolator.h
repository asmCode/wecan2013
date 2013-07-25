#pragma once

#include "IKeysContainer.h"

template <class Type>
class IInterpolator : public IKeysContainer
{
public:
	virtual void GetValue(float time, Type &value) = 0;
	virtual void AddKeyframe(float time, Type value, bool stopKey) = 0;
	virtual void GetKeyframe(int index, float &time, Type &value, bool &stopKey) = 0;
};
