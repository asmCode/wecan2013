#pragma once

#include <Graphics/Property.h>

class PropertySignal
{
public:
	PropertySignal(Property *prop);
	
	bool CheckSignal(float time);

private:
	Property *m_property;

	int m_lastValue;
};

