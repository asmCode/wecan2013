#pragma once

#include "Interpolators\StateInterpolator.h"
#include "Interpolators\TCBInterpolator.h"
#include "Interpolators\LinearInterpolator.h"

#include <Math/Vec3.h>

#include <string>
#include <stdint.h>
#include <assert.h>

class Property
{
public:
	enum PropertyType
	{
		PropertyType_Boolean,
		PropertyType_Int,
		PropertyType_Float,
		PropertyType_Vector3,
		PropertyType_String
	};

	enum AnimationType
	{
		AnimationType_None,		// no animation
		AnimationType_State,	// state animation, no interpolation between keyframes
		AnimationType_Linear,	// linear animation for floats and vectors
		AnimationType_TCB		// Kochanek–Bartels spline interpolation for floats and vectors
	};

	Property(
		const std::string &name,
		PropertyType propType,
		AnimationType animType) :
		
		m_name(name),
		m_propType(propType),
		m_animType(animType),
		m_value(NULL)
	{
		InitProperty();
	}

	~Property()
	{
		if (m_value != NULL)
			delete m_value;
	}

	bool IsAnimatable() const
	{
		return m_animType != AnimationType_None;
	}

	unsigned int GetKeysCount() const
	{
		assert(m_animType != AnimationType_None);

		return ((IKeysContainer*)m_value)->GetKeysCount();
	}

	template <typename Type>
	IInterpolator<Type> *GetInterpolator()
	{
		assert(m_animType != AnimationType_None);

		return (IInterpolator<Type>*)m_value;
	}


	const char* GetName() const
	{
		return m_name.c_str();
	}

	PropertyType GetPropertyType() const
	{
		return m_propType;
	}

	AnimationType GetAnimationType() const
	{
		return m_animType;
	}

	void SetValue(bool value)
	{
		assert(m_propType == PropertyType_Boolean);
		memcpy(m_value, &value, sizeof(bool));
	}

	void SetValue(int value, float time = 0)
	{
		assert(m_propType == PropertyType_Int);
		
		if (m_animType == AnimationType_None)
			memcpy(m_value, &value, sizeof(int));
		else
		{
			((Interpolator<int>*)m_value)->AddKeyframe(time, value, false);
		}
	}

	void SetValue(float value, float time = 0)
	{
		assert(m_propType == PropertyType_Float);

		if (m_animType == AnimationType_None)
			memcpy(m_value, &value, sizeof(float));
		else
		{
			((Interpolator<float>*)m_value)->AddKeyframe(time, value, false);
		}
	}

	void SetValue(sm::Vec3 value)
	{
		assert(m_propType == PropertyType_Vector3);
		memcpy(m_value, &value, sizeof(sm::Vec3));
	}

	void SetValue(const std::string &value)
	{
		assert(m_propType == PropertyType_String);
		*((std::string*)m_value) = value;
	}

	bool GetBoolValue()
	{
		assert(m_propType == PropertyType_Boolean);
		bool value;
		memcpy(&value, m_value, sizeof(bool));
		return value;
	}

	int GetIntValue(float time = 0.0f, int *keyframeIndex = NULL)
	{
		assert(m_propType == PropertyType_Int);

		int value;

		if (!IsAnimatable())
		{
			memcpy(&value, m_value, sizeof(int));
		}
		else
		{
			int lastKeyframeIndex = ((IInterpolator<int>*)m_value)->GetValue(time, value);
			if (keyframeIndex != NULL)
				*keyframeIndex = lastKeyframeIndex;
		}

		return value;
	}

	float GetFloatValue(float time = 0.0f)
	{
		assert(m_propType == PropertyType_Float);

		float value;

		if (!IsAnimatable())
		{
			memcpy(&value, m_value, sizeof(float));
		}
		else
		{
			((IInterpolator<float>*)m_value)->GetValue(time, value);
		}

		return value;
	}

	template <typename Type>
	void GetKeyframe(int index, float &time, Type &value, bool &stopKey)
	{
		assert(IsAnimatable());

		((IInterpolator<Type>*)m_value)->GetKeyframe(index, time, value, stopKey);
	}

	sm::Vec3 GetVector3Value()
	{
		assert(m_propType == PropertyType_Vector3);
		sm::Vec3 value;
		memcpy(&value, m_value, sizeof(sm::Vec3));
		return value;
	}

	const std::string GetStringValue()
	{
		assert(m_propType == PropertyType_String);
		return ((std::string*)m_value) ->c_str();
	}

private:
	std::string m_name;
	PropertyType m_propType;
	AnimationType m_animType;
	void *m_value;

	// this initialization sux and sould be rewritten
	void InitProperty()
	{
		if (!IsAnimatable())
		{
			switch (m_propType)
			{
			case PropertyType_Boolean: m_value = new bool(); break;
			case PropertyType_Int: m_value = new int(); break;
			case PropertyType_Float: m_value = new float(); break;
			case PropertyType_Vector3: m_value = new sm::Vec3(); break;
			case PropertyType_String: m_value = new std::string(); break;
			}
		}
		else
		{
			//InterpolatorFactory::InterpolationFunction interMethod;

			/*switch (m_animType)
			{
			case AnimationType_State: interMethod = InterpolatorFactory::InterpolationMethod_State;
			case AnimationType_Linear: interMethod = InterpolatorFactory::InterpolationMethod_Linear;
			case AnimationType_TCB: interMethod = InterpolatorFactory::InterpolationMethod_TCB;
			}*/

			switch (m_animType)
			{
			case AnimationType_State:
				switch (m_propType)
				{
				case PropertyType_Boolean: m_value = new StateInterpolator<bool>(); break;
				case PropertyType_Int: m_value = new StateInterpolator<int>(); break;
				case PropertyType_Float: m_value = new StateInterpolator<float>(); break;
				case PropertyType_Vector3: m_value = new StateInterpolator<sm::Vec3>(); break;
				case PropertyType_String: m_value = new StateInterpolator<std::string>(); break;
				}

				break;

			case AnimationType_Linear:
				switch (m_propType)
				{
				case PropertyType_Boolean: assert(0); break;
				case PropertyType_Int: m_value = new LinearInterpolator<int>(); break;
				case PropertyType_Float: m_value = new LinearInterpolator<float>(); break;
				case PropertyType_Vector3: m_value = new LinearInterpolator<sm::Vec3>(); break;
				case PropertyType_String: assert(0); break;
				}

				break;

			case AnimationType_TCB:
				switch (m_propType)
				{
				case PropertyType_Boolean: assert(0); break;
				case PropertyType_Int: m_value = new TCBInterpolator<int>(); break;
				case PropertyType_Float: m_value = new TCBInterpolator<float>(); break;
				case PropertyType_Vector3: m_value = new TCBInterpolator<sm::Vec3>(); break;
				case PropertyType_String: assert(0); break;
				}

				break;
			}
		}
	}
};

