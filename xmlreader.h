#pragma once
#include"rapidxml\rapidxml_utils.hpp"
#include<memory>

class XmlReader
{
private:
	std::unique_ptr<rapidxml::xml_document<>> pDoc;
	std::unique_ptr<rapidxml::file<>> pFile;
	rapidxml::xml_node<>* currentXmlNode;
protected:
	bool gotoFirstChildElement(const char* tag);
	bool gotoNextSiblingElement();
	bool gotoParent();

	bool getIntValue(int& toValue);
	bool getDoubleValue(double& toValue);
	bool getStringValue(std::string& toValue);
public:
	XmlReader(const char* fileName);
	~XmlReader();

	
};