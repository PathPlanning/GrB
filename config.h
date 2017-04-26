#pragma once
#include"configxmlreader.h"
#include"gl_const.h"

class ConfigBuilder;

class Config
{
private:
	double *searchParams;
	int paramsCount = CN_NPT;

	void set(int index, double value)
	{
		if (index < paramsCount)
		{
			searchParams[index] = value;
		}
	}

protected:
public:
	friend ConfigBuilder;

	Config()
	{
		searchParams = new double[paramsCount];
	}

	~Config()
	{
		if (searchParams)
		{
			delete[] searchParams;
		}
	}

	inline double get(int index)
	{
		if (index < paramsCount)
		{
			return searchParams[index];
		}

		return -1;
	}
};

typedef std::shared_ptr<Config> config_ptr;