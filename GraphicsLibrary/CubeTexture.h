#pragma once

#include <string>
#include <stdint.h>

class CubeTexture
{
public:
	CubeTexture(const std::string &texNamePrefix);
	~CubeTexture(void);

	uint32_t GetTextureId() const;

private:
	uint32_t m_texId;
};

