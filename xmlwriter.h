#pragma once
#include<memory>
#include"rapidxml\rapidxml_utils.hpp"

class XmlWriter
{
private:
	std::unique_ptr<rapidxml::xml_document<>> pDoc;
	std::unique_ptr<rapidxml::file<>> pFile;
	std::unique_ptr<rapidxml::xml_node<>> rootElement;

	rapidxml::xml_node<>* currentXmlNode;

	std::string fileName;
public:
	XmlWriter(const char* fName);
	~XmlWriter();

	void save();

	std::string getFileName()
	{
		return fileName;
	}
	
	bool gotoFirstChildElement(const char* tag);
	bool gotoLastChildElement(const char* tag);
	bool gotoNextSiblingElement();
	bool gotoParent();
	bool gotoRoot();
	bool setCurrentElementAsRoot();
	bool addNewElement(const char* tag);
	bool addNewElement(const char* tag, std::string value);
	bool addNewElement(const char* tag, const char* value);
	bool addAttribute(const char* name, const char* value);
	bool addAttribute(const char* name, int& value);
	bool addAttribute(const char* name, unsigned int& value);
	bool addAttribute(const char* name, double& value);
	bool addAttribute(const char* name, bool& value);
};
