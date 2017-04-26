#pragma once
#include<map>
#include<vector>
#include<queue>
#include<deque>
#include"searchresult.h"
#include"map.h"
#include"config.h"
#include"ilogger.h"
#include"node.h"
#include"iavoidobst.h"

class GrB
{
private:
	map_ptr map;
	logger_ptr logger;
	config_ptr config;
	avoidobst_ptr avoidObstAlg;

	unsigned int peakNumberOfCreatedSection = 0;
	unsigned int numberOfUniqueCreatedSection = 0;
	int numberofsteps = 0;
	double bestPathLength = 0;

	std::map<Cell, node_ptr, CellComparator> OPEN;
	std::map<Cell, node_ptr, CellComparator> CLOSE;
	std::vector<Cell> nullTraj;
	std::vector<Cell> bestPlan;
	std::vector<Cell> bestPath;
	std::vector<std::vector<Cell>> paths;

	void addOpen(node_ptr node);
	void addClose(node_ptr node);
	void removeOpen(node_ptr node);

	node_ptr createNodeAndAddToOpen(const Cell& cell, const Cell& goal);
	node_ptr createNodeAndTryAddToClose(const Cell& cell, const Cell& goal);

	int getNumberOfFoundPaths(node_ptr goal);
	std::vector<Cell> getFinalPlan(node_ptr goal);
	std::vector<Cell> getFinalPath(const std::vector<Cell>& plan);

	node_ptr findMin();
	void createNullTrajectory(const Cell& start, const Cell& goal);
	bool findObstacle(int &i_obstacle, int &j_obstacle, int &numberOfCellOnTheObst);

	bool getGoodPPlan();
	void fixPlanCandidates(node_ptr start, node_ptr goal, std::vector<std::vector<Cell>>& plans);
	void expandAllNodesWithChangedGValues(std::queue<node_ptr>& nodeList);


	//to find all paths
	int numberOfPath = 0;
	void findAllPaths(node_ptr goal, bool needToSavePaths);

	node_ptr startNode;
	node_ptr goalNode;
protected:
public:
	GrB();
	~GrB();
	SearchResult startSearch(map_ptr m, logger_ptr l, config_ptr c, avoidobst_ptr a);
};