#include"mission.h"
#include"mapbuilder.h"
#include"configbuilder.h"
#include"loggerfactory.h"
#include"baseavoidobst.h"
#include"grb.h"

Mission::Mission(const char* fileName)
{
	FileName = fileName;
}

Mission::~Mission()
{
}

bool Mission::getMap()
{
	MapBuilder builder(FileName);
	map = builder.build();
	
	return map != nullptr;
}

bool Mission::getConfig()
{
	ConfigBuilder builder(FileName);
	config = builder.build();

	return config != nullptr;
}

bool Mission::getLog()
{
	LoggerFactory factory(FileName);
	logger = factory.create();

	return logger != nullptr;
}

bool Mission::getAvoidObstacleAlgorithm()
{
	avoidObstAlg = std::make_shared<BaseAvoidObst>(map, logger);
	return true;
}

bool Mission::startSearch()
{
	GrB search;
	sResult = search.startSearch(map, logger, config, avoidObstAlg);
	return sResult.pathfound;
}

void Mission::saveSearchResultsToLog() 
{
	if (sResult.pathfound)
	{
		if (config->get(CN_PT_NPP) > 0)
		{
			sResult.pathsFound = sResult.paths.size() / config->get(CN_PT_NPP);
		}
		else
		{
			sResult.pathsFound = 1;
		}

		logger->writeToLogMap(map, sResult.path);
		logger->writeToLogAllPaths(sResult);
	}
	else
	{
		sResult.pathsFound = 0;
	}

	logger->writeToLogSummary(sResult);

	logger->save();
}

void Mission::printSearchResultsToConsole()
{
	std::cout << "Path ";
	if (!sResult.pathfound)
		std::cout << "NOT ";
	std::cout << "found!" << std::endl;

	std::cout << "numberofsteps=" << sResult.numberofsteps << std::endl;
	std::cout << "sectionscreated=" << sResult.sectionscreated << std::endl;
	std::cout << "peaksectionscreated=" << sResult.peaksectionscreated << std::endl;

	if (!sResult.pathfound)
		std::cout << "pathlength=" << sResult.pathlength << std::endl;
	std::cout << "time=" << sResult.time << std::endl;
}