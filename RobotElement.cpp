#include "RobotElement.h"

bool RobotElement::IsCam()
{
	return false;
}

int RobotElement::GetId()
{
	return 0;
}

sm::Matrix& RobotElement::Transform()
{
	return m_transform;
}

sm::Matrix& RobotElement::AnimTransform()
{
	return m_animTransform;
}

