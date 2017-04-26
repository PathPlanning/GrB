#include"xmlwriter.h"
#include<sstream>
#include"rapidxml\rapidxml_print.hpp"

XmlWriter::XmlWriter(const char* fName)
{
	std::string saveFileName(fName);
	auto index = saveFileName.find_last_of(".");
	if (index != std::string::npos)
	{
		saveFileName = saveFileName.insert(index, "_log");
	}
	else
	{
		saveFileName = saveFileName.append("_log");
	}

	fileName = saveFileName;

	pFile = std::unique_ptr<rapidxml::file<>>(new rapidxml::file<>(fName));
	pDoc = std::unique_ptr<rapidxml::xml_document<>>(new rapidxml::xml_document<>());
	pDoc->parse<rapidxml::parse_default>(pFile->data());

	currentXmlNode = pDoc.get();
}

XmlWriter::~XmlWriter()
{
	if (rootElement != nullptr)
	{
		rootElement.release();
	}

	if (pDoc != nullptr)
	{
		pDoc.release();
	}

	if (pFile != nullptr)
	{
		pFile.release();
	}
}

bool XmlWriter::gotoFirstChildElement(const char* tag)
{
	if (!currentXmlNode)
	{
		return false;
	}

	auto searchedXmlNode = currentXmlNode->first_node(tag);

	if (searchedXmlNode)
	{
		currentXmlNode = searchedXmlNode;
	}

	return searchedXmlNode != nullptr;
}

bool XmlWriter::gotoNextSiblingElement()
{
	if (!currentXmlNode)
	{
		return false;
	}

	auto searchedXmlNode = currentXmlNode->next_sibling();
	
	if (searchedXmlNode)
	{
		currentXmlNode = searchedXmlNode;
	}

	return searchedXmlNode != nullptr;
}

bool XmlWriter::gotoLastChildElement(const char* tag)
{
	if (!currentXmlNode)
	{
		return false;
	}

	auto searchedXmlNode = currentXmlNode->last_node(tag);

	if (searchedXmlNode)
	{
		currentXmlNode = searchedXmlNode;
	}

	return searchedXmlNode != nullptr;
}

bool XmlWriter::gotoParent()
{
	if (!currentXmlNode)
	{
		return false;
	}
	auto searchedXmlNode = currentXmlNode->parent();

	if (searchedXmlNode)
	{
		currentXmlNode = searchedXmlNode;
	}

	return searchedXmlNode != nullptr;
}

void XmlWriter::save()
{
	std::ofstream savedFile(fileName);
	savedFile << *pDoc.get();
	savedFile.close();
	pDoc->clear();
}

bool XmlWriter::setCurrentElementAsRoot()
{
	if (currentXmlNode != nullptr)
	{
		rootElement = std::unique_ptr<rapidxml::xml_node<>>(currentXmlNode);
		return true;
	}

	return false;
}

bool XmlWriter::addNewElement(const char* tag)
{
	if (currentXmlNode == nullptr)
	{
		return false;
	}

	currentXmlNode->append_node(pDoc->allocate_node(rapidxml::node_element, tag));
	return true;
}

bool XmlWriter::addNewElement(const char* tag, const char* value)
{
	if (currentXmlNode == nullptr)
	{
		return false;
	}

	char *cValue = pDoc->allocate_string(value);

	currentXmlNode->append_node(pDoc->allocate_node(rapidxml::node_element, tag, cValue));
	return true;
}

bool XmlWriter::addNewElement(const char* tag, std::string value)
{
	return addNewElement(tag, value.c_str());
}

bool XmlWriter::addAttribute(const char* name, const char* value)
{
	if (currentXmlNode == nullptr)
	{
		return false;
	}

	char *cValue = pDoc->allocate_string(value);
	currentXmlNode->append_attribute(pDoc->allocate_attribute(name, cValue));
	return true;
}

bool XmlWriter::addAttribute(const char* name, int& value)
{
	return addAttribute(name, std::to_string(value).c_str());
}

bool XmlWriter::addAttribute(const char* name, unsigned int& value)
{
	return addAttribute(name, std::to_string(value).c_str());
}

bool XmlWriter::addAttribute(const char* name, double& value) 
{
	return addAttribute(name, std::to_string(value).c_str());
}

bool XmlWriter::addAttribute(const char* name, bool& value) 
{
	return addAttribute(name, value ? "true" : "false");
}

bool XmlWriter::gotoRoot()
{
	if (rootElement == nullptr)
	{
		return false;
	}

	currentXmlNode = rootElement.get();
	return true;
}

