#pragma once

class IUpdateable
{
public:
	virtual void Update(float time, float seconds) = 0;
};
