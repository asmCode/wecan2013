#pragma once

#include "IKeysContainer.h"

template <class Type>
class IInterpolator : public IKeysContainer
{
public:
	virtual int GetValue(float time, Type &value, int lastKeyframeIndex = 0) = 0;
	virtual void AddKeyframe(float time, Type value, bool stopKey) = 0;
	virtual void InsertKeyframe(float time, Type value, bool stopKey) = 0;
	virtual void GetKeyframe(int index, float &time, Type &value, bool &stopKey) = 0;
};
