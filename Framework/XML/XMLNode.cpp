#include "XMLNode.h"
#include "../Utils/StringUtils.h"

#include <stdio.h>
#include <assert.h>

XMLNode::XMLNode()
{
}

XMLNode::XMLNode(const std::string &name) :
	m_name(name)
{
}

XMLNode::XMLNode(const std::string &name, const std::string &value) :
	m_name(name),
	m_value(value)
{
}

XMLNode::~XMLNode()
{
	for (uint32_t i = 0; i < m_children.size(); i++)
		delete m_children[i];
}

std::string XMLNode::GetName() const
{
	return m_name;
}

std::string XMLNode::GetValueAsString() const
{
	return m_value;
}

uint32_t XMLNode::GetValueAsUInt32() const
{
	return ParseValue<uint32_t>(m_value, "%ud");
}

int32_t XMLNode::GetValueAsInt32() const
{
	return ParseValue<int32_t>(m_value, "%d");
}

float XMLNode::GetValueAsFloat() const
{
	return ParseValue<float>(m_value, "%f");
}

bool XMLNode::GetValueAsBool() const
{
	if (StringUtils::LowerCase(m_value) == "true" || m_value == "1")
		return true;
	else if (StringUtils::LowerCase(m_value) == "false" || m_value == "0")
		return false;
	else
	{
		assert(false && "its not a bool");
		return false;
	}
}

bool XMLNode::HasAttrib(const std::string &name)
{
	std::map<std::string, std::string>::iterator it = m_attribs.find(name);
	if (it == m_attribs.end())
		return false;

	return true;
}

std::string XMLNode::GetAttribAsString(const std::string &name)
{
	return m_attribs[name];
}

uint32_t XMLNode::GetAttribAsUInt32(const std::string &name)
{
	return ParseValue<uint32_t>(m_attribs[name], "%ud");
}

int32_t XMLNode::GetAttribAsInt32(const std::string &name)
{
	return ParseValue<int32_t>(m_attribs[name], "%d");
}

float XMLNode::GetAttribAsFloat(const std::string &name)
{
	return ParseValue<float>(m_attribs[name], "%f");
}

bool XMLNode::GetAttribAsBool(const std::string &name)
{
	if (StringUtils::LowerCase(m_attribs[name]) == "true" || m_value == "1")
		return true;
	else if (StringUtils::LowerCase(m_attribs[name]) == "false" || m_value == "0")
		return false;
	else
	{
		assert(false && "its not a bool");
		return false;
	}
}

uint32_t XMLNode::GetChildrenCount() const
{
	return m_children.size();
}

XMLNode& XMLNode::operator[](const std::string &name) const
{
	for (uint32_t i = 0; i < m_children.size(); i++)
		if (m_children[i]->m_name == name)
			return *m_children[i];

	assert(false && "no such child element");
	return *(new XMLNode()); // only to remove warning
}

XMLNode& XMLNode::operator[](uint32_t index) const
{
	assert(index < m_children.size());

	return *m_children[index];
}

