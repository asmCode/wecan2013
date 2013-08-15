#include "PropertySignal.h"
#include <assert.h>

PropertySignal::PropertySignal(Property *prop) :
	m_property(prop),
	m_lastValue(0)
{
	assert(m_property != NULL);
	assert(m_property->GetAnimationType() == Property::AnimationType_State);
	assert(m_property->GetPropertyType() == Property::PropertyType_Int);
}
	
bool PropertySignal::CheckSignal(float time)
{
	int currentVal = m_property->GetIntValue(time);
	if (currentVal != m_lastValue)
	{
		m_lastValue = currentVal;
		return true;
	}

	return false;
}

