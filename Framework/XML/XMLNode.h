#ifndef XMLNODE_H
#define XMLNODE_H

#include <string>
#include <map>
#include <vector>
#include <stdint.h>
#include <assert.h>

class XMLNode
{
	friend class XMLLoader;

public:
	XMLNode();
	XMLNode(const std::string &name);
	XMLNode(const std::string &name, const std::string &value);

	~XMLNode();

	std::string GetName() const;

	std::string GetValueAsString() const;
	uint32_t GetValueAsUInt32() const;
	int32_t GetValueAsInt32() const;
	float GetValueAsFloat() const;
	bool GetValueAsBool() const;

	bool HasAttrib(const std::string &name);
	std::string GetAttribAsString(const std::string &name);
	uint32_t GetAttribAsUInt32(const std::string &name);
	int32_t GetAttribAsInt32(const std::string &name);
	float GetAttribAsFloat(const std::string &name);
	bool GetAttribAsBool(const std::string &name);

	uint32_t GetChildrenCount() const;
	XMLNode& operator[](const std::string &name) const;
	XMLNode& operator[](uint32_t index) const;

private:
	std::string m_name;
	std::string m_value;

	std::map<std::string, std::string> m_attribs;

	std::vector<XMLNode*> m_children;

	template <typename Type> Type ParseValue(const std::string &value, const char *format) const
	{
		Type result;
		if (sscanf(value.c_str(), format, &result) != 1)
		{
			assert(false && "wrong format");
		}
		return result;
	}
};

#endif // XMLNODE_H

