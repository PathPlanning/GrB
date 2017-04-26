#pragma once
#include"baselogger.h"
#include"ilogger.h"
#include"deeplogger.h"
#include"logxmlreader.h"
#include"gl_const.h"

class LoggerFactory
{
private:
	const char* fileName;
protected:
public:
	LoggerFactory(const char* fName)
	{
		fileName = fName;
	}

	~LoggerFactory()
	{
	}

	logger_ptr create()
	{
		LogXmlReader reader(fileName);

		logger_ptr logger = nullptr;

		int loglevel;
		if (!reader.getLogLevel(loglevel))
		{
			loglevel = CN_LOGLVL_NO;
		}


		if (loglevel == CN_LOGLVL_HIGH)
		{
			logger = std::make_shared<BaseLogger>(fileName);
		}
		else if (loglevel >= CN_LOGLVL_LOW)
		{
			logger = std::make_shared<DeepLogger>(fileName);
		}
		else
		{
			logger = std::make_shared<NullLogger>();
		}
		

		return logger;
	}
};