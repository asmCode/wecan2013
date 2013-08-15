#pragma once

#include <string>

class IContentObserver
{
public:
	virtual void ErrorOccured(const std::string &errorMsg) {};
	virtual void OnLoadingContentStarted(int stepsCount) {};
	virtual void OnProgressStep() {};
	virtual void OnLoadingFinished() {};
};
