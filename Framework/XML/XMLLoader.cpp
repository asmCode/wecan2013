#include "XMLLoader.h"
#include "XMLNode.h"
#include <tinyxml/tinyxml2.h>

XMLNode* XMLLoader::LoadNode(const tinyxml2::XMLElement *element)
{
	std::string name = element->Name() != NULL ? element->Name() : "";
	std::string value = element->GetText() != NULL ? element->GetText() : "";
	XMLNode *node = new XMLNode(name, value);

	const tinyxml2::XMLAttribute *attrib = element->FirstAttribute();
	while (attrib != NULL)
	{
		node->m_attribs[attrib->Name()] = attrib->Value();
		attrib = attrib->Next();
	}

	const tinyxml2::XMLElement *subElement = element->FirstChildElement();
	while (subElement != NULL)
	{
		node->m_children.push_back(LoadNode(subElement));
		subElement = subElement->NextSiblingElement();
	}

	return node;
}

XMLNode* XMLLoader::LoadFromFile(const std::string &path)
{
	tinyxml2::XMLDocument xmlDoc;
	if (xmlDoc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR)
		return NULL;
	
	const tinyxml2::XMLElement *rootElement = xmlDoc.RootElement();
	if (rootElement == NULL)
		return NULL;

	return LoadNode(rootElement);
}

