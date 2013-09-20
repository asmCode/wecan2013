#pragma once

#include <vector>

class MeshPart;

class GameObject
{
public:
	virtual ~GameObject() {}

	virtual void Awake() {}

	virtual void Update(float time, float seconds) {}
	virtual void Draw() {}

	virtual std::vector<MeshPart*>& GetMeshParts();

	virtual void ClearLightmaps() {}

private:
	static std::vector<MeshPart*> m_dummy;
};

