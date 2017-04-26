#include"xmlreader.h"
#include"rapidxml\rapidxml_utils.hpp"
#include<iostream>
#include<fstream>


XmlReader::XmlReader(const char* fileName)
{
	pFile = std::unique_ptr<rapidxml::file<>>(new rapidxml::file<>(fileName));
	pDoc = std::unique_ptr<rapidxml::xml_document<>>(new rapidxml::xml_document<>()); 
	pDoc->parse<rapidxml::parse_default>(pFile->data());
	currentXmlNode = pDoc.get();
}


XmlReader::~XmlReader()
{
	if (pFile != nullptr)
	{
		pFile.release();
	}

	if (pDoc != nullptr)
	{
		pDoc.release();
	}

	currentXmlNode = nullptr;
}

bool XmlReader::gotoFirstChildElement(const char* tag)
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

bool XmlReader::gotoNextSiblingElement()
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

bool XmlReader::gotoParent()
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

bool XmlReader::getIntValue(int& toValue)
{
	std::string value;
	if (!getStringValue(value))
	{
		return false;
	}

	toValue = std::stoi(value);

	return true;
}

bool XmlReader::getDoubleValue(double& toValue)
{
	std::string value;
	if (!getStringValue(value))
	{
		return false;
	}

	toValue = std::stod(value);

	return true;
}

bool XmlReader::getStringValue(std::string& toValue)
{
	if (!currentXmlNode)
	{
		return false;
	}

	std::string value(currentXmlNode->value());
	toValue = value;

	return !value.empty();
}






