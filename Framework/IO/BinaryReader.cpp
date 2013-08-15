#include "BinaryReader.h"

BinaryReader::BinaryReader(const void *mem) :
	m_mem(mem)
{
}

void BinaryReader::ReadBuffer(char *buff, unsigned size)
{
	memcpy(buff, m_mem, size);
	m_mem = (char*)m_mem + size;
}

template <>
std::string BinaryReader::Read<std::string>()
{
	std::string val;
	unsigned len = Read<unsigned>();
	if (len > 0)
	{
		char *str = new char[len];
		ReadBuffer(str, len);
		val.assign(str, len);
		delete [] str;
	}
	return val;
}

