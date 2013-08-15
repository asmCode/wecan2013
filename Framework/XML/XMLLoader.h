#ifndef XML_LOADER_H
#define XML_LOADER_H

#include <string>

class XMLNode;
namespace tinyxml2 { class XMLElement; }

class XMLLoader
{
public:
	static XMLNode* LoadFromFile(const std::string &path);

private:
	static XMLNode* LoadNode(const tinyxml2::XMLElement *element);
};

#endif

