#include"configxmlreader.h"

ConfigXmlReader::ConfigXmlReader(const char* fileName) : XmlReader(fileName)
{
	gotoFirstChildElement("root");
	gotoFirstChildElement("algorithm");
}

ConfigXmlReader::~ConfigXmlReader()
{
}

bool ConfigXmlReader::getMetricType(std::string& toValue)
{
	if (!gotoFirstChildElement("metrictype"))
	{
		return false;
	}

	bool res = getStringValue(toValue);
	gotoParent();

	return res;
}

bool ConfigXmlReader::getMaxSteps(int& toValue)
{
	if (!gotoFirstChildElement("maxsteps"))
	{
		return false;
	}

	bool res = getIntValue(toValue);
	gotoParent();

	return res;
}

bool ConfigXmlReader::getNumberOfPPlans(int& toValue)
{
	if (!gotoFirstChildElement("pplans"))
	{
		return false;
	}

	bool res = getIntValue(toValue);
	gotoParent();

	return res;
}

bool ConfigXmlReader::getWeight(double& toValue)
{
	if (!gotoFirstChildElement("hweight"))
	{
		return false;
	}

	bool res = getDoubleValue(toValue);
	gotoParent();

	return res;
}