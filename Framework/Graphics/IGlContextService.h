#pragma once
#ifndef _IGLCONTEXTSERVICE_
#define _IGLCONTEXTSERVICE_

#include <windows.h>
#include <gl/GL.h>

class IGlContextService
{
public:
	virtual ~IGlContextService() {}
	virtual HGLRC GetContext() = 0;
};

#endif
