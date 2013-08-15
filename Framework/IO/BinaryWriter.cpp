#include "BinaryWriter.h"

BinaryWriter::BinaryWriter(std::ostream *os) :
	os(os)
{
}

void BinaryWriter::Write(const void *buffer, uint32_t size)
{
	os->write(static_cast<const char*>(buffer), size);
}

template <>
void BinaryWriter::Write(const std::string &value)
{
	Write<uint32_t>(static_cast<uint32_t>(value.size()));
	if (value.size() > 0)
		os->write(value.c_str(), value.size());
}

