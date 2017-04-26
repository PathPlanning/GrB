#pragma once

#include <vector>
#include <memory>
#include "cell.h"
class IAvoidObst
{
public:
	virtual ~IAvoidObst() {}

	virtual std::vector<std::vector<Cell>> getAvoidObstaclePlans(	std::vector<Cell> nullTraj, int numberOfNodeOnTheObst, const Cell& start, const Cell& goal, unsigned int& maxNumberOfCreatedNode) = 0;
};

typedef std::shared_ptr<IAvoidObst> avoidobst_ptr;