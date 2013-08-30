#pragma once

#include <stdint.h>

class Billboard
{
public:
	static bool Initialize();
	static void Release();

	static void Setup();
	static void Clean();

	static void Draw();

private:
	static uint32_t m_vboId;
	static float m_vertices[8];
};

