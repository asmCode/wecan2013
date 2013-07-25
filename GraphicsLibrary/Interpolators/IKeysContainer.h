#pragma once

class IKeysContainer
{
public:
	virtual ~IKeysContainer() {}
	virtual int GetKeysCount() = 0;
	virtual void DeleteKey(int index) = 0;
};
