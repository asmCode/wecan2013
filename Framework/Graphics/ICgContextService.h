#pragma once
#ifndef _ICGCONTEXTSERVICE_
#define _ICGCONTEXTSERVICE_

class ICgContextService
{
public:
	virtual ~ICgContextService() {}
	virtual CGcontext GetContext() = 0;
};

#endif
