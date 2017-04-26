#pragma once
#include"xmlreader.h"

class ConfigXmlReader : XmlReader
{
private:
protected:
public:
	ConfigXmlReader(const char* fileName);
	~ConfigXmlReader();

	bool getMetricType(std::string& toValue);
	bool getNumberOfPPlans(int& toValue);
	bool getMaxSteps(int& toValue);
	bool getWeight(double& toValue);
};