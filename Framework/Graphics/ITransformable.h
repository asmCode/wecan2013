#pragma once

#include <Math/Matrix.h>

class ITransformable
{
public:
	virtual bool IsCam() = 0;
	virtual int GetId() = 0;
	virtual sm::Matrix& Transform() = 0;
	virtual sm::Matrix& AnimTransform() = 0;
	virtual sm::Matrix& BaseTransform() = 0;
};
