#ifndef ROBOT_ELEMENT
#define ROBOT_ELEMENT

#include <Math/Matrix.h>
#include <Graphics/ITransformable.h>
#include <vector>

class Model;
class MeshPart;

class RobotElement : public ITransformable
{
public:
	Model *m_model;
	sm::Matrix m_transform;
	sm::Matrix m_animTransform;

	std::vector<MeshPart*> m_meshParts;

	bool IsCam();
	int GetId();
	sm::Matrix& Transform();
	sm::Matrix& AnimTransform();

private:
};

#endif // ROBOT_ELEMENT

