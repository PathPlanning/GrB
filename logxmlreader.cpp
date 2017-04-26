#include"logxmlreader.h"

LogXmlReader::LogXmlReader(const char* fileName) : XmlReader(fileName)
{
	gotoFirstChildElement("root");
	gotoFirstChildElement("options");
}

LogXmlReader::~LogXmlReader()
{
}

bool LogXmlReader::getLogLevel(int& toValue)
{
	if (!gotoFirstChildElement("loglevel"))
	{
		return false;
	}
	bool res = getIntValue(toValue);
	gotoParent();

	return res;
}
