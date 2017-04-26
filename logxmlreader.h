#pragma once
#include"xmlreader.h"

class LogXmlReader : XmlReader
{
private:
protected:
public:
	LogXmlReader(const char* fileName);
	~LogXmlReader();

	bool getLogLevel(int& toValue);
};