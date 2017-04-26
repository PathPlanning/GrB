#pragma once
#include"cell.h"
#include<vector>

struct SearchResult
{
	bool pathfound;
	double pathlength;
	std::vector<Cell> path;
	std::vector<Cell> plan;
	std::vector<std::vector<Cell>> paths;
	unsigned int sectionscreated;
	unsigned int peaksectionscreated;
	float clength;
	float hlength;
	unsigned int numberofsteps;
	double time;
	double sumLength;
	double pathsFound;


	SearchResult()
	{
		pathfound = false;
		pathlength = 0;
		sectionscreated = 0;
		peaksectionscreated = 0;
		clength = 0;
		hlength = 0;
		numberofsteps = 0;
		time = 0;
	}

};