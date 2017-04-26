#pragma once
#include<iostream>
#include"config.h"
#include"configxmlreader.h"
#include<algorithm>
#include"utils.h"

class ConfigBuilder
{
private:
	const char* fileName;
protected:
public:
	ConfigBuilder(const char* fName)
	{
		fileName = fName;
	}

	~ConfigBuilder()
	{}

	config_ptr build()
	{
		ConfigXmlReader reader(fileName);

		config_ptr config = std::make_shared<Config>();

		{ // metric type
			std::string metrictype;
			if (!reader.getMetricType(metrictype))
			{
				std::cout << "No 'metrictype' element found in 'algorithm' section.\n";
				metrictype = CNS_EUCL;
			}

			std::transform(metrictype.begin(), metrictype.end(), metrictype.begin(), ::tolower);

			if (metrictype == CNS_EUCL)
			{
				config->set(CN_PT_MT, CN_MT_EUCL);
				METRICTYPE = CN_MT_EUCL;
			}
			else if (metrictype == CNS_MANH)
			{
				config->set(CN_PT_MT, CN_MT_MANH);
				METRICTYPE = CN_MT_MANH;
			}
			else if (metrictype == CNS_DIAG)
			{
				config->set(CN_PT_MT, CN_MT_DIAG);
				METRICTYPE = CN_MT_DIAG;
			}
		}

		{//number of pplans
			int pplans;
			if (!reader.getNumberOfPPlans(pplans))
			{
				std::cout << "No 'pplans' element found in 'algorithm' section.\n";
				pplans = -1;
			}

			if (pplans < 0)
			{
				std::cout << "There is no limit on number of pplans.\n";
			}

			config->set(CN_PT_NPP, pplans);
		}

		{//weight
			double weight;
			if (!reader.getWeight(weight))
			{
				std::cout << "No 'weight' element found in 'algorithm' section. weight = 1\n";
				weight = 1;
			}
			config->set(CN_PT_WEIGHT, weight);
			WEIGHT = weight;
		}

		{// max steps for debug
			int maxsteps;
			if (!reader.getMaxSteps(maxsteps))
			{
				std::cout << "No 'maxsteps' element found in XML file" << std::endl;
				maxsteps = -1;
			}
			config->set(CN_PT_MAXSTEPS, maxsteps);
		}

		return config;
	}
};