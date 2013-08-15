#ifndef BINARY_WRITER
#define BINARY_WRITER

#include <ostream>
#include <stdint.h>

class BinaryWriter
{
public:
	BinaryWriter(std::ostream *os);

	template <typename T>
	void Write(const T &value)
	{
		os->write(reinterpret_cast<const char*>(&value), sizeof(T));
	}

	void Write(const void *buffer, uint32_t size);

private:
	std::ostream *os;
};

template<>
void BinaryWriter::Write<std::string>(const std::string&);

#endif // BINARY_WRITER

