#pragma once

template <typename Type>
class KeyFrame
{
public:
	float time;
	bool stopKey;
	Type value;
};
