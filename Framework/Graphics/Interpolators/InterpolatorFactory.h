#pragma once

#include <typeinfo>
#include "IInterpolator.h"

#include "StateInterpolator.h"
#include "TCBInterpolator.h"
#include "LinearInterpolator.h"
#include <assert.h>

class InterpolatorFactory
{
public:
	enum InterpolationFunction
	{
		InterpolationMethod_Linear,
		InterpolationMethod_State,
		InterpolationMethod_TCB,
	};

	template <typename Type>
	static IInterpolator<Type>* CreateInterpolator(InterpolationFunction func)
	{
		switch (func)
		{
		case InterpolationMethod_Linear:
			return new LinearInterpolator<Type>();

		case InterpolationMethod_State:
			return new StateInterpolator<Type>();

		case InterpolationMethod_TCB:
			return new TCBInterpolator<Type>();
		}

		return NULL;
	}
};
