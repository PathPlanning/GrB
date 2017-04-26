#pragma once
#include"map.h"
#include"config.h"
#include"ilogger.h"
#include"iavoidobst.h"
#include<memory>

class Mission
{
private:
	map_ptr map;
	config_ptr config;
	logger_ptr logger;
	avoidobst_ptr avoidObstAlg;
	/*Search search;*/
	SearchResult sResult;
	const char* FileName;

public:
	Mission(const char* fileName);
	~Mission();

	bool getMap();
	bool getConfig();
	bool getLog();
	bool getAvoidObstacleAlgorithm();
	bool startSearch();
	void saveSearchResultsToLog();
	void printSearchResultsToConsole();
	/*
	void createSearch();
	void createEnvironmentOptions();
	void startSearch();
	void printSearchResultsToConsole();
	void saveSearchResultsToLog();*/
};