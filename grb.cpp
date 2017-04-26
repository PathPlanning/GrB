#include"grb.h"
#include"node.h"
#include<deque>
#include<stack>
#include<iostream>

GrB::GrB()
{
}

GrB::~GrB()
{
}

SearchResult GrB::startSearch(map_ptr m, logger_ptr l, config_ptr c, avoidobst_ptr a)
{
	map = m;
	logger = l;
	config = c;
	avoidObstAlg = a;

	SearchResult result;

	startTimer();

	if (getGoodPPlan())
	{
		result.pathfound = true;
		result.pathlength = bestPathLength;
		result.plan = bestPlan;
		result.path = bestPath;
		result.time = getTime();
		result.paths = paths;
	}
	else
	{ 
		result.pathfound = false;
	}

	logger->writeToLogNodes(OPEN, CLOSE);

	stopTimer();
	result.time = getTime();
	result.sectionscreated = OPEN.size() + CLOSE.size();
	result.peaksectionscreated = peakNumberOfCreatedSection;
	result.numberofsteps = numberofsteps;

	return result;
}

bool GrB::getGoodPPlan()
{
	node_ptr goal = MAKE_NODE(map->getGoal(), 0);
	node_ptr start = MAKE_NODE(map->getStart(), calculateH(map->getStart(), map->getGoal()));

	startNode = start;
	goalNode = goal;

	goal->setProtectToAddNext();
	start->setProtectToAddPrev();

	OPEN[start->getCell()] = start;

	bool gValueWasUpdated = false;
	goal->addPrev(start, gValueWasUpdated);
	start->addNext(goal);

	bool stop = false;

	bool isAtLeastOnePathWasFound = false;

	while (true)
	{
		logger->writeToLogNodes(OPEN, CLOSE);

		if (config->get(CN_PT_MAXSTEPS) != -1 && config->get(CN_PT_MAXSTEPS) <= numberofsteps)
		{
			return false;
		}

		numberofsteps++;

		auto curNode = findMin();

		if (curNode == nullptr)
		{
			if(isAtLeastOnePathWasFound)
			{
				bestPlan = getFinalPlan(goal);
				bestPathLength = goal->getG();
				bestPath = getFinalPath(bestPlan);
				findAllPaths(goal, true);

				return true;
			}
			else
			{
				return false;
			}
		}

		createNullTrajectory(curNode->getCell(), goal->getCell());

		int i_obstacle, j_obstacle, numberofCellOnTheObst;
		if (findObstacle(i_obstacle, j_obstacle, numberofCellOnTheObst))
		{
			unsigned int numberOfNodesForAvoidObst = 0;
			auto plans = avoidObstAlg->getAvoidObstaclePlans(nullTraj, numberofCellOnTheObst, curNode->getCell(), goal->getCell(), numberOfNodesForAvoidObst);

			if (peakNumberOfCreatedSection < OPEN.size() + CLOSE.size() + numberOfNodesForAvoidObst)
			{
				peakNumberOfCreatedSection = OPEN.size() + CLOSE.size() + numberOfNodesForAvoidObst;
			}

			if (plans.empty())
			{
				return false;
			}
			
			fixPlanCandidates(curNode, goal, plans);
		}
		else
		{
			removeOpen(curNode);
			addClose(curNode);

			isAtLeastOnePathWasFound = true;

			if (config->get(CN_PT_NPP) == -1)
			{
				if (OPEN.empty())
				{
					bestPlan = getFinalPlan(goal);
					bestPathLength = goal->getG();
					bestPath = getFinalPath(bestPlan);
					findAllPaths(goal, true);

					return true;
				}
			}
			else if (config->get(CN_PT_NPP) == 1)
			{
				bestPlan = getFinalPlan(goal);
				bestPathLength = goal->getG();
				bestPath = getFinalPath(bestPlan);
				return true;
			}
			else
			{
				findAllPaths(goal, true);
				if (numberOfPath >= config->get(CN_PT_NPP) || OPEN.empty())
				{
					bestPlan = getFinalPlan(goal);
					bestPathLength = goal->getG();
					bestPath = getFinalPath(bestPlan);

					return true;
				}
			}
		}
	}

	return false;
}

void GrB::fixPlanCandidates(node_ptr start, node_ptr goal, std::vector<std::vector<Cell>>& plans)
{
	start->removeNext(goal);
	goal->removeParent(start);

	removeOpen(start);
	addClose(start);

	std::queue<node_ptr> nodeWithUpdatedGValue;

	bool gValueWasUpdated = false;
	for (unsigned int i = 0; i < plans.size(); ++i)
	{
		auto plan = plans[i];

		node_ptr nodeToUpdateData = start;
		for (unsigned int j = 0; j < plan.size(); ++j)
		{
			Cell cell = plan[j];

			node_ptr savedNode = nullptr;
			if(j == plan.size() - 1)
			{
				savedNode = createNodeAndAddToOpen(cell, goal->getCell());
			}
			else
			{ 
				savedNode = createNodeAndTryAddToClose(cell, goal->getCell());
			}

			nodeToUpdateData->addNext(savedNode);
			savedNode->addPrev(nodeToUpdateData, gValueWasUpdated);

			if (gValueWasUpdated)
			{
				nodeWithUpdatedGValue.push(savedNode);
			}
			nodeToUpdateData = savedNode;
		}

		if(OPEN.find(nodeToUpdateData->getCell()) != std::end(OPEN))
		{
			nodeToUpdateData->addNext(goal);
			goal->addPrev(nodeToUpdateData, gValueWasUpdated);
		}
	}

	expandAllNodesWithChangedGValues(nodeWithUpdatedGValue);
}

void GrB::expandAllNodesWithChangedGValues(std::queue<node_ptr>& nodeList)
{
	bool gValueWasUpdated = false;

	while (!nodeList.empty())
	{
		node_ptr node = nodeList.front();
		nodeList.pop();

		for(auto nodeIter : node->getNext())
		{
			node_ptr nextNode = nodeIter.second;
			nextNode->updateParent(node->getCell(), gValueWasUpdated);
			if (gValueWasUpdated)
			{
				nodeList.push(nextNode);
			}
		}
	}
}

void GrB::createNullTrajectory(const Cell& start, const Cell& goal)
{
	nullTraj.clear();
	brezenheim(start, goal, nullTraj);
}

bool GrB::findObstacle(int &i_obstacle, int &j_obstacle, int &numberOfCellOnTheObst)
{
	int vectsize = nullTraj.size();
	int x, y;

	for (int j = 1; j < vectsize; j++)
	{
		x = nullTraj[j].i;
		y = nullTraj[j].j;

		if (map->get(x,y) == CN_OBSTL)
		{
			i_obstacle = x;
			j_obstacle = y;

			numberOfCellOnTheObst = j;

			return true;
		}
		else
		{
			int prevX = nullTraj[j - 1].i;
			int prevY = nullTraj[j - 1].j;

			if (abs(x - prevX) == 1 && abs(y - prevY) == 1)
			{
				if (map->get(x, prevY) == CN_OBSTL && map->get(prevX, y) == CN_OBSTL)
				{
					i_obstacle = x;
					j_obstacle = y;

					numberOfCellOnTheObst = j;

					return true;
				}
			}
		}
	}

	return false;
}

node_ptr GrB::findMin()
{
	if (OPEN.empty())
	{
		return nullptr;
	}

	auto bestNode = begin(OPEN)->second;

	for (auto iter = std::next(begin(OPEN)); iter != end(OPEN); ++iter)
	{
		auto node = iter->second;

		if (node->getF() < bestNode->getF())
		{
			bestNode = node;
		}
		else if (node->getF() == bestNode->getF() &&
			node->getH() < bestNode->getH())
		{
			bestNode = node;
		}
	}

	return bestNode;
}

void GrB::addOpen(node_ptr node)
{
	OPEN.insert(std::make_pair(node->getCell(), node));
}

void GrB::addClose(node_ptr node)
{
	CLOSE.insert(std::make_pair(node->getCell(), node));
}

void GrB::removeOpen(node_ptr node)
{
	OPEN.erase(node->getCell());
}

node_ptr GrB::createNodeAndAddToOpen(const Cell& cell, const Cell& goal)
{
	auto iter = OPEN.find(cell);

	if (iter != end(OPEN))
	{
		return iter->second;
	}

	node_ptr node = nullptr;

	iter = CLOSE.find(cell);
	if (iter != end(CLOSE))
	{
		node = iter->second;
		//CLOSE.erase(cell);
		//OPEN.insert(std::make_pair(cell, node));
		return node;
	}

	if(node == nullptr)
	{
		node = MAKE_NODE(cell, calculateH(cell, goal));
	}

	OPEN.insert(std::make_pair(cell, node));

	return node;
}

node_ptr GrB::createNodeAndTryAddToClose(const Cell& cell, const Cell& goal)
{
	auto iter = CLOSE.find(cell);

	if (iter != end(CLOSE))
	{
		return iter->second;
	}

	iter = OPEN.find(cell);
	if (iter != end(OPEN))
	{
		return iter->second;
	}

	auto node = MAKE_NODE(cell, calculateH(cell, goal));
	CLOSE.insert(std::make_pair(cell, node));

	return node;
}

std::vector<Cell> GrB::getFinalPlan(node_ptr goal)
{
	std::vector<Cell> plan;

	node_ptr curNode = goal->getBestParent();

	plan.insert(begin(plan), goal->getCell());
	plan.insert(begin(plan), curNode->getCell());

	while (!curNode->isParentListEmpty())
	{
		node_ptr nodePtr = curNode->getBestParent();
		
		if (nodePtr == nullptr)
		{
			break;
		}

		curNode = nodePtr;
		plan.insert(begin(plan), curNode->getCell());
	}

	return plan;
}

std::vector<Cell> GrB::getFinalPath(const std::vector<Cell>& plan)
{
	std::vector<Cell> result;
	nullTraj.clear();

	for (unsigned int i = 1; i < plan.size(); i++)
	{
		Cell startCell = plan[i - 1];
		Cell finishCell = plan[i];

		brezenheim(startCell, finishCell, nullTraj);

		if (i != (plan.size() - 1))
			nullTraj.pop_back();

		result.insert(result.end(), nullTraj.begin(), nullTraj.end());
		nullTraj.clear();
	}

	return result;
}

void GrB::findAllPaths(node_ptr goal, bool needToSavePaths)
{
	numberOfPath = 0;
	std::deque<Cell> path;
	paths.clear();

	std::map<Cell, int, CellComparator> cellPresentedInPath;
	std::map<Cell, std::stack<int>, CellComparator> branchingIndexes;

	std::stack<node_ptr> nodes;
	path.push_back(goal->getCell());
	for (auto parent : goal->getPrevList())
	{
		Cell parentCell = parent.first;
		node_ptr parentNode = parent.second->get();

		if (OPEN.find(parentCell) != end(OPEN))
		{
			continue;
		}
		branchingIndexes[parentCell].push(path.size());
		nodes.push(parentNode);
	}
	

	while (!nodes.empty())
	{
		node_ptr node = nodes.top();
		nodes.pop();

		Cell cell = node->getCell();
		int branchIndex = branchingIndexes[cell].top();
		branchingIndexes[cell].pop();

		if (path.size() > branchIndex)
		{
			unsigned int numberToRemove = path.size() - branchIndex;
			for (unsigned int i = 0; i < numberToRemove; ++i)
			{
				Cell cellToRemove = path.back();
				path.pop_back();
				cellPresentedInPath.erase(cellToRemove);
			}
		}

		if (cell == startNode->getCell())
		{
			if (needToSavePaths)
			{
				std::vector<Cell> newPath;

				newPath.push_back(cell);
				for (auto iter = path.rbegin(); iter != path.rend(); ++iter)
				{
					newPath.push_back(*iter);
				}

				paths.push_back(newPath);
			}

			numberOfPath++;
			if (numberOfPath == 1000)
			{
				return;
			}
			continue;
		}

		if (cellPresentedInPath.find(cell) != cellPresentedInPath.end())
		{
			continue;
		}

		cellPresentedInPath[node->getCell()] = path.size();
		path.push_back(node->getCell());

		for (auto parent : node->getPrevList())
		{
			Cell parentCell = parent.first;
			node_ptr parentNode = parent.second->get();
			branchingIndexes[parentCell].push(path.size());
			nodes.push(parentNode);
		}
	}
}