#include"baseavoidobst.h"
#include"utils.h"

BaseAvoidObst::BaseAvoidObst(map_ptr map, logger_ptr logger) : map(map), logger(logger)
{
	coordOffsetCheckObst[CN_OBSTPOS_RIGHT] = CoordOffset(0, 1);
	coordOffsetCheckObst[CN_OBSTPOS_LEFT] = CoordOffset(0, -1);
	coordOffsetCheckObst[CN_OBSTPOS_UP] = CoordOffset(-1, 0);
	coordOffsetCheckObst[CN_OBSTPOS_DOWN] = CoordOffset(1, 0);

	coordOffsetCurrentMovement[CN_COUR_RIGHT] = CoordOffset(0, 1);
	coordOffsetCurrentMovement[CN_COUR_LEFT] = CoordOffset(0, -1);
	coordOffsetCurrentMovement[CN_COUR_UP] = CoordOffset(-1, 0);
	coordOffsetCurrentMovement[CN_COUR_DOWN] = CoordOffset(1, 0);

	externalRotation[RT_UP_RIGHT] = MovementData(CN_COUR_RIGHT, CN_OBSTPOS_DOWN);
	externalRotation[RT_UP_LEFT] = MovementData(CN_COUR_LEFT, CN_OBSTPOS_DOWN);
	externalRotation[RT_DOWN_RIGHT] = MovementData(CN_COUR_RIGHT, CN_OBSTPOS_UP);
	externalRotation[RT_DOWN_LEFT] = MovementData(CN_COUR_LEFT, CN_OBSTPOS_UP);
	externalRotation[RT_RIGHT_DOWN] = MovementData(CN_COUR_DOWN, CN_OBSTPOS_LEFT);
	externalRotation[RT_RIGHT_UP] = MovementData(CN_COUR_UP, CN_OBSTPOS_LEFT);
	externalRotation[RT_LEFT_DOWN] = MovementData(CN_COUR_DOWN, CN_OBSTPOS_RIGHT);
	externalRotation[RT_LEFT_UP] = MovementData(CN_COUR_UP, CN_OBSTPOS_RIGHT);

	internalRotation[RT_UP_RIGHT] = MovementData(CN_COUR_LEFT, CN_OBSTPOS_UP);
	internalRotation[RT_UP_LEFT] = MovementData(CN_COUR_RIGHT, CN_OBSTPOS_UP);
	internalRotation[RT_DOWN_RIGHT] = MovementData(CN_COUR_LEFT, CN_OBSTPOS_DOWN);
	internalRotation[RT_DOWN_LEFT] = MovementData(CN_COUR_RIGHT, CN_OBSTPOS_DOWN);
	internalRotation[RT_RIGHT_DOWN] = MovementData(CN_COUR_UP, CN_OBSTPOS_RIGHT);
	internalRotation[RT_RIGHT_UP] = MovementData(CN_COUR_DOWN, CN_OBSTPOS_RIGHT);
	internalRotation[RT_LEFT_DOWN] = MovementData(CN_COUR_UP, CN_OBSTPOS_LEFT);
	internalRotation[RT_LEFT_UP] = MovementData(CN_COUR_DOWN, CN_OBSTPOS_LEFT);

	inverseRotation[RT_UP_RIGHT] = RT_LEFT_DOWN;
	inverseRotation[RT_RIGHT_DOWN] = RT_UP_LEFT;
	inverseRotation[RT_DOWN_LEFT] = RT_RIGHT_UP;
	inverseRotation[RT_LEFT_UP] = RT_DOWN_RIGHT;
	inverseRotation[RT_NONE] = RT_NONE;
}

BaseAvoidObst::~BaseAvoidObst()
{
}

std::vector<std::vector<Cell>> BaseAvoidObst::getAvoidObstaclePlans(std::vector<Cell> nullTraj, int numberOfNodeOnTheObst, const Cell& s, const Cell& g, unsigned int& maxNumberOfCreatedAONode)
{
	std::vector<std::vector<Cell>> result;
	std::vector<AONode> cornerAONodes;

	start_i = nullTraj[numberOfNodeOnTheObst - 1].i;
	start_j = nullTraj[numberOfNodeOnTheObst - 1].j;

	start = s;
	finish = g;

	cur_i = start_i;
	cur_j = start_j;

	std::vector<AONode> rotationPoints;
	rotationPoints.clear();

	getSectionMovingCourse(start.i, start.j, finish.i, finish.j);

	getStartClockwiseCourse();

	CourseMoving startCourse = course;
	ObstaclePosition startObstPosition = obstPosition;

	containsAllRotationPoint = getAllRotationPoints(rotationPoints);

	maxNumberOfCreatedAONode = rotationPoints.size();

	if (containsAllRotationPoint)
	{
		//by direct rotation
		cur_i = start_i;
		cur_j = start_j;
		course = startCourse;
		obstPosition = startObstPosition;

		cornerAONodes.clear();

        if (buildPathByDirectRotationPointList(rotationPoints, cornerAONodes, 0))
		{
			result.push_back(createAvoidPlan(cornerAONodes));
		}

		//by inverse rotation
		cur_i = start_i;
		cur_j = start_j;
		cornerAONodes.clear();

		getStartCounterClockwiseCourse();

        if (buildPathByInverseRotationPointList(rotationPoints, cornerAONodes, rotationPoints.size() - 1))
		{
			result.push_back(createAvoidPlan(cornerAONodes));
		}
	}
	else
	{
		//clockwise
		cur_i = start_i;
		cur_j = start_j;
		course = startCourse;
		obstPosition = startObstPosition;

		cornerAONodes.clear();

		std::vector<AONode> clockwiseRotationPoints = rotationPoints;
		std::vector<AONode> counterClockwiseRotationPoints;

		rotationPoints.clear();

		//couterclockwise
		cur_i = start_i;
		cur_j = start_j;
		getSectionMovingCourse(start.i, start.j, finish.i, finish.j);
		getStartCounterClockwiseCourse();

		getAllRotationPoints(rotationPoints);

		if (rotationPoints.size() > maxNumberOfCreatedAONode)
		{
			maxNumberOfCreatedAONode = rotationPoints.size();
		}

		counterClockwiseRotationPoints = rotationPoints;
		rotationPoints.clear();

		cur_i = start_i;
		cur_j = start_j;

        if (buildPathByDirectRotationWithHalfPointsList(clockwiseRotationPoints, counterClockwiseRotationPoints, cornerAONodes, 0))
		{
			result.push_back(createAvoidPlan(cornerAONodes));
		}

		cornerAONodes.clear();

        if (buildPathByDirectRotationWithHalfPointsList(counterClockwiseRotationPoints, clockwiseRotationPoints, cornerAONodes, 0))
		{
			result.push_back(createAvoidPlan(cornerAONodes));
		}

		cornerAONodes.clear();
	}
	return result;
}

void BaseAvoidObst::getSectionMovingCourse(int start_i, int start_j, int finish_i, int finish_j)
{
	HorizontalCourse = getHorizontalMovingCourse(start_j, finish_j);
	VerticalCourse = getVerticalMovingCourse(start_i, finish_i);
}

CourseMoving BaseAvoidObst::getHorizontalMovingCourse(int start_j, int finish_j)
{
	CourseMoving horizontalCourse;

	double delta_j = start_j - finish_j;

	if (delta_j <= 0)
		horizontalCourse = CN_COUR_RIGHT;
	else
		horizontalCourse = CN_COUR_LEFT;

	return horizontalCourse;
}

CourseMoving BaseAvoidObst::getVerticalMovingCourse(int start_i, int finish_i)
{
	CourseMoving verticalCourse;

	double delta_i = start_i - finish_i;

	if (delta_i < 0)
		verticalCourse = CN_COUR_DOWN;
	else
		verticalCourse = CN_COUR_UP;

	return verticalCourse;
}

void BaseAvoidObst::getStartClockwiseCourse()
{
	if (VerticalCourse == CN_COUR_DOWN)
	{
		if (HorizontalCourse == CN_COUR_RIGHT)
		{
			if (map->get(cur_i + 1, cur_j) != 0 && map->get(cur_i, cur_j + 1) != 0)
			{
				course = CN_COUR_UP;
				obstPosition = CN_OBSTPOS_RIGHT;


			}
			else if (map->get(cur_i + 1, cur_j) != 0)
			{
				course = CN_COUR_RIGHT;
				obstPosition = CN_OBSTPOS_DOWN;
			}
			else if (map->get(cur_i, cur_j + 1) != 0)
			{
				course = CN_COUR_UP;
				obstPosition = CN_OBSTPOS_RIGHT;
			}
			else
			{
				cur_j += 1;
				course = CN_COUR_RIGHT;
				obstPosition = CN_OBSTPOS_DOWN;
			}
		}
		else
		{
			if (map->get(cur_i + 1, cur_j) != 0 && map->get(cur_i, cur_j - 1) != 0)
			{
				course = CN_COUR_RIGHT;
				obstPosition = CN_OBSTPOS_DOWN;
			}
			else if (map->get(cur_i + 1, cur_j) != 0)
			{
				course = CN_COUR_RIGHT;
				obstPosition = CN_OBSTPOS_DOWN;
			}
			else if (map->get(cur_i, cur_j - 1) != 0)
			{
				course = CN_COUR_DOWN;
				obstPosition = CN_OBSTPOS_LEFT;
			}
			else
			{
				cur_i += 1;
				course = CN_COUR_DOWN;
				obstPosition = CN_OBSTPOS_LEFT;
			}
		}
	}
	else
	{
		if (HorizontalCourse == CN_COUR_RIGHT)
		{
			if (map->get(cur_i - 1, cur_j) != 0 && map->get(cur_i, cur_j + 1) != 0)
			{
				course = CN_COUR_LEFT;
				obstPosition = CN_OBSTPOS_UP;


			}
			else if (map->get(cur_i - 1, cur_j) != 0)
			{
				course = CN_COUR_LEFT;
				obstPosition = CN_OBSTPOS_UP;
			}
			else if (map->get(cur_i, cur_j + 1) != 0)
			{
				course = CN_COUR_UP;
				obstPosition = CN_OBSTPOS_RIGHT;
			}
			else
			{
				cur_i -= 1;
				course = CN_COUR_UP;
				obstPosition = CN_OBSTPOS_RIGHT;
			}
		}
		else
		{
			if (map->get(cur_i - 1, cur_j) != 0 && map->get(cur_i, cur_j - 1) != 0)
			{
				course = CN_COUR_DOWN;
				obstPosition = CN_OBSTPOS_LEFT;


			}
			else if (map->get(cur_i - 1, cur_j) != 0)
			{
				course = CN_COUR_LEFT;
				obstPosition = CN_OBSTPOS_UP;
			}
			else if (map->get(cur_i, cur_j - 1) != 0)
			{
				course = CN_COUR_DOWN;
				obstPosition = CN_OBSTPOS_LEFT;
			}
			else
			{
				cur_j -= 1;
				course = CN_COUR_LEFT;
				obstPosition = CN_OBSTPOS_UP;
			}
		}
	}
}

void BaseAvoidObst::getStartCounterClockwiseCourse()
{
	if (VerticalCourse == CN_COUR_DOWN)
	{
		if (HorizontalCourse == CN_COUR_RIGHT)
		{
			if (map->get(cur_i + 1, cur_j) != 0 && map->get(cur_i, cur_j + 1) != 0)
			{
				course = CN_COUR_LEFT;
				obstPosition = CN_OBSTPOS_DOWN;


			}
			else if (map->get(cur_i + 1, cur_j) != 0)
			{
				course = CN_COUR_LEFT;
				obstPosition = CN_OBSTPOS_DOWN;
			}
			else if (map->get(cur_i, cur_j + 1) != 0)
			{
				course = CN_COUR_DOWN;
				obstPosition = CN_OBSTPOS_RIGHT;
			}
			else
			{
				cur_i += 1;
				course = CN_COUR_DOWN;
				obstPosition = CN_OBSTPOS_RIGHT;
			}
		}
		else
		{
			if (map->get(cur_i + 1, cur_j) != 0 && map->get(cur_i, cur_j - 1) != 0)
			{
				course = CN_COUR_UP;
				obstPosition = CN_OBSTPOS_LEFT;


			}
			else if (map->get(cur_i + 1, cur_j) != 0)
			{
				course = CN_COUR_LEFT;
				obstPosition = CN_OBSTPOS_DOWN;
			}
			else if (map->get(cur_i, cur_j - 1) != 0)
			{
				course = CN_COUR_UP;
				obstPosition = CN_OBSTPOS_LEFT;
			}
			else
			{
				cur_j -= 1;
				course = CN_COUR_LEFT;
				obstPosition = CN_OBSTPOS_DOWN;
			}
		}
	}
	else
	{
		if (HorizontalCourse == CN_COUR_RIGHT)
		{
			if (map->get(cur_i - 1, cur_j) != 0 && map->get(cur_i, cur_j + 1) != 0)
			{
				course = CN_COUR_DOWN;
				obstPosition = CN_OBSTPOS_RIGHT;


			}
			else if (map->get(cur_i - 1, cur_j) != 0)
			{
				course = CN_COUR_RIGHT;
				obstPosition = CN_OBSTPOS_UP;
			}
			else if (map->get(cur_i, cur_j + 1) != 0)
			{
				course = CN_COUR_DOWN;
				obstPosition = CN_OBSTPOS_RIGHT;
			}
			else
			{
				cur_j += 1;
				course = CN_COUR_RIGHT;
				obstPosition = CN_OBSTPOS_UP;
			}
		}
		else
		{
			if (map->get(cur_i - 1, cur_j) != 0 && map->get(cur_i, cur_j - 1) != 0)
			{
				course = CN_COUR_RIGHT;
				obstPosition = CN_OBSTPOS_UP;


			}
			else if (map->get(cur_i - 1, cur_j) != 0)
			{
				course = CN_COUR_RIGHT;
				obstPosition = CN_OBSTPOS_UP;
			}
			else if (map->get(cur_i, cur_j - 1) != 0)
			{
				course = CN_COUR_UP;
				obstPosition = CN_OBSTPOS_LEFT;
			}
			else
			{
				cur_i -= 1;
				course = CN_COUR_UP;
				obstPosition = CN_OBSTPOS_LEFT;
			}
		}
	}
}

bool BaseAvoidObst::getAllRotationPoints(std::vector<AONode>& rotationPoints)
{
	RotationType rotationEndingMovement = (RotationType)(course + obstPosition);

	CourseMoving startCourse = course;
	ObstaclePosition startObstPosition = obstPosition;

	int iOffsetCheckObstacle = coordOffsetCheckObst[obstPosition].iOffset;
	int jOffsetCheckObstacle = coordOffsetCheckObst[obstPosition].jOffset;

	int iOffsetCurMove = coordOffsetCurrentMovement[course].iOffset;
	int jOffsetCurMove = coordOffsetCurrentMovement[course].jOffset;

	bool stop = false;

	while (!stop)
	{
		if (map->get(cur_i, cur_j) == 1)
		{
			cur_i -= iOffsetCurMove;
			cur_j -= jOffsetCurMove;

			ObstaclePosition nextObstPos = internalRotation[rotationEndingMovement].obstPos;

            if (!addRotationPointIfPossible(rotationPoints, AONode(cur_i, cur_j, RT_NONE, obstPosition, nextObstPos)))
			{
				return true;
			}
			//rotationPoints.push_back(AONode(cur_i, cur_j, RT_NONE, obstPosition, nextObstPos));

			course = internalRotation[rotationEndingMovement].course;
			obstPosition = nextObstPos;

			rotationEndingMovement = (RotationType)(course + obstPosition);

			iOffsetCheckObstacle = coordOffsetCheckObst[obstPosition].iOffset;
			jOffsetCheckObstacle = coordOffsetCheckObst[obstPosition].jOffset;

			iOffsetCurMove = coordOffsetCurrentMovement[course].iOffset;
			jOffsetCurMove = coordOffsetCurrentMovement[course].jOffset;
		}
		else if (map->get(cur_i + iOffsetCheckObstacle, cur_j + jOffsetCheckObstacle) == 0)
		{
			ObstaclePosition nextObstPosition = externalRotation[rotationEndingMovement].obstPos;

            if (!addRotationPointIfPossible(rotationPoints, AONode(cur_i, cur_j, rotationEndingMovement, obstPosition, nextObstPosition)))
			{
				return true;
			}
			//rotationPoints.push_back(AONode(cur_i, cur_j, rotationEndingMovement, obstPosition, nextObstPosition));

			course = externalRotation[rotationEndingMovement].course;
			obstPosition = nextObstPosition;

			rotationEndingMovement = (RotationType)(course + obstPosition);

			if (course == startCourse && obstPosition == startObstPosition)
			{
				if (cur_i == start_i && cur_j == start_j)
				{
					stop = true;
				}
			}

			cur_j += jOffsetCheckObstacle;
			cur_i += iOffsetCheckObstacle;

			iOffsetCheckObstacle = coordOffsetCheckObst[obstPosition].iOffset;
			jOffsetCheckObstacle = coordOffsetCheckObst[obstPosition].jOffset;

			iOffsetCurMove = coordOffsetCurrentMovement[course].iOffset;
			jOffsetCurMove = coordOffsetCurrentMovement[course].jOffset;
		}
		else
		{
			cur_i += iOffsetCurMove;
			cur_j += jOffsetCurMove;
		}

		if (cur_i < 0 || cur_i >= map->getHeight() || cur_j < 0 || cur_j >= map->getWidth())
		{
			cur_i -= iOffsetCurMove;
			cur_j -= jOffsetCurMove;

			if (rotationPoints.size() != 0)
			{
				Cell cell = rotationPoints.rbegin()->cell;
				if (cell.i != cur_i || cell.j != cur_j)
				{
					rotationPoints.push_back(AONode(cur_i, cur_j, RT_NONE, obstPosition, CN_OBSTPOS_NONE));
				}
			}
			else
			{
				rotationPoints.push_back(AONode(cur_i, cur_j, RT_NONE, obstPosition, CN_OBSTPOS_NONE));
			}

			return false;
		}

		/*if (cur_i == start_i && cur_j == start_j)
		{
			if (course == startCourse && obstPosition == startObstPosition)
			{
				stop = true;
			}
		}*/
	}

	return true;
}

bool BaseAvoidObst::addRotationPointIfPossible(std::vector<AONode>& rotationPoints, AONode newRotationPoint)
{
	if (rotationPoints.empty())
	{
		rotationPoints.push_back(newRotationPoint);
		return true;
	}

	AONode firstNode = rotationPoints[0];

	if (firstNode == newRotationPoint)
	{
		return false;
	}

	rotationPoints.push_back(newRotationPoint);
	return true;
}

bool BaseAvoidObst::buildPathByDirectRotationPointList(std::vector<AONode>& rotationPoint, std::vector<AONode>& cornerAONodes, unsigned int startPosition)
{
    for (; startPosition < rotationPoint.size(); startPosition++)
	{
		AONode curAONode = rotationPoint[startPosition];
		cornerAONodes.push_back(curAONode);

		std::vector<Cell> nullTraj;
		nullTraj.clear();

		brezenheim(curAONode.cell, finish, nullTraj);

		int posOfIntersection;

        if (!checkIntersectionNullTrajAndCurrentObstacle(nullTraj, rotationPoint, posOfIntersection, curAONode))
		{
			return true;
		}
	}

	return false;
}

bool BaseAvoidObst::buildPathByDirectRotationWithHalfPointsList(std::vector<AONode>& rotationPoint, std::vector<AONode>& rPointsForCheck, std::vector<AONode>& cornerAONodes, unsigned int startPosition)
{
    for (; startPosition < rotationPoint.size(); startPosition++)
	{
		AONode curAONode = rotationPoint[startPosition];
		cornerAONodes.push_back(curAONode);

		std::vector<Cell> nullTraj;
		nullTraj.clear();

		brezenheim(curAONode.cell, finish, nullTraj);

		int posOfIntersection;

        if (!checkIntersectionNullTrajAndCurrentObstacle(nullTraj, rotationPoint, posOfIntersection, curAONode))
		{
            if (!checkIntersectionNullTrajAndCurrentObstacle(nullTraj, rPointsForCheck, posOfIntersection, curAONode))
			{
				return true;
			}
		}
	}

	return false;
}

bool BaseAvoidObst::buildPathByInverseRotationPointList(std::vector<AONode>& rotationPoint, std::vector<AONode>& cornerAONodes, int startPosition)
{
    for (; startPosition >= 0; startPosition--)
	{
		AONode curAONode = rotationPoint[startPosition];
		curAONode.rotate = inverseRotation[curAONode.rotate];
		cornerAONodes.push_back(curAONode);

		std::vector<Cell> nullTraj;
		nullTraj.clear();

		brezenheim(curAONode.cell, finish, nullTraj);

		int posOfIntersection;

        if (!checkIntersectionNullTrajAndCurrentObstacle(nullTraj, rotationPoint, posOfIntersection, curAONode))
		{
			return true;				
		}
	}

	return false;
}

bool BaseAvoidObst::checkIntersectionNullTrajAndCurrentObstacle(std::vector<Cell>& nullTraj, std::vector<AONode>& list, int& pos, AONode& curAONode)
{
	int vectsize = nullTraj.size();
	if (checkIntersectionNullTrajAndObstacles(nullTraj))
	{
		pos = findPositionInListOfIntersectionWithSelectedAONode(list, nullTraj[0], nullTraj[vectsize - 1], false, curAONode);
		if (pos != -1)
		{
			return true;
		}
	}
	return false;
}

bool BaseAvoidObst::checkIntersectionNullTrajAndObstacles(std::vector<Cell>& nullTraj)
{
	int vectsize = nullTraj.size();
	int x, y;

	for (int i = 1; i < vectsize; i++)
	{
		x = nullTraj[i].i;
		y = nullTraj[i].j;

		if (map->get(x, y) == CN_OBSTL)
		{
			return true;
		}
		else
		{
			int prevX = nullTraj[i - 1].i;
			int prevY = nullTraj[i - 1].j;

			if (abs(x - prevX) == 1 && abs(y - prevY) == 1)
			{
				if (map->get(x, prevY) == CN_OBSTL && map->get(prevX, y) == CN_OBSTL)
				{
					return true;
				}
			}
		}
	}

	return false;
}

int BaseAvoidObst::findPositionInListOfIntersectionWithSelectedAONode(std::vector<AONode>& list, Cell& start, Cell& finish, bool needToCheckObstPosition, AONode& curAONode)
{
	int pos = -1;

	std::vector<ObstaclePosition> possibleObstPosition;
	if (needToCheckObstPosition)
	{
		possibleObstPosition = getPossibleObstPositions(start, finish);
	}

	if (list.empty())
	{
		return -1;
	}

	if (!containsAllRotationPoint)
	{
		if (list[0] != curAONode)
		{
			ObstaclePosition obstPos = list[0].obstPositionNext;
            Cell cell(start_i, start_j);
            if (checkIsLinePartsIntersectedInCorrectDirection(cell, list[0].cell, start, finish, possibleObstPosition, obstPos))
			{
				return 0;
			}
		}
	}

	for (pos = 1; pos < list.size(); ++pos)
	{
		if ((list[pos - 1] == curAONode || list[pos] == curAONode))
			continue;
		ObstaclePosition obstPosition = list[pos].obstPositionPrev;
		if (checkIsLinePartsIntersectedInCorrectDirection(list[pos - 1].cell, list[pos].cell, start, finish, possibleObstPosition, obstPosition))
		{
			return pos;
		}
	}

	if (containsAllRotationPoint)
	{
		if ((list[list.size() - 1] != curAONode && list[0] != curAONode))
		{
			ObstaclePosition obstPos = list[list.size() - 1].obstPositionNext;
			if (checkIsLinePartsIntersectedInCorrectDirection(list[list.size() - 1].cell, list[0].cell, start, finish, possibleObstPosition, obstPos))
			{
				return 0;
			}
		}
	}

	return -1;
}

std::vector<ObstaclePosition> BaseAvoidObst::getPossibleObstPositions(Cell& start, Cell& finish)
{
	std::vector<ObstaclePosition> res;

	int y_spacing = start.i - finish.i;
	int x_spacing = start.j - finish.j;

	if (y_spacing > 0)
	{
		res.push_back(CN_OBSTPOS_UP);
	}
	else if (y_spacing < 0)
	{
		res.push_back(CN_OBSTPOS_DOWN);
	}

	if (x_spacing > 0)
	{
		res.push_back(CN_OBSTPOS_LEFT);
	}
	else if (x_spacing < 0)
	{
		res.push_back(CN_OBSTPOS_RIGHT);
	}

	return res;
}

bool inline BaseAvoidObst::checkIsLinePartsIntersectedInCorrectDirection(Cell& cell1, Cell& cell2, Cell& start, Cell& finish, std::vector<ObstaclePosition>& possibleObstPosition, ObstaclePosition& curObstPosition)
{
	if (IsLinePartsIntersected(cell1, cell2, start, finish))
	{
		if (possibleObstPosition.size() > 0)
		{
			for (unsigned int i = 0; i < possibleObstPosition.size(); ++i)
			{
				if (possibleObstPosition[i] == curObstPosition)
				{
					return true;
				}
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}

bool BaseAvoidObst::IsLinePartsIntersected(Cell a, Cell b, Cell c, Cell d)
{
	if (a.i == b.i && a.j == b.j)
	{
		return IsPointBelogToLine(a, c, d);
	}

	double common = (b.j - a.j)*(d.i - c.i) - (b.i - a.i)*(d.j - c.j);

	if (common == 0)
	{
		if (a.j - b.j == 0 && c.j - d.j == 0 && a.j == c.j)
		{
			if ((std::min)(a.i, b.i) <= (std::max)(c.i, d.i) && (std::max)(a.i, b.i) >= (std::min)(c.i, d.i))
			{
				return true;
			}
		}
		else if (a.i - b.i == 0 && c.i - d.i == 0 && a.i == c.i)
		{
			if ((std::min)(a.j, b.j) <= (std::max)(c.j, d.j) && (std::max)(a.j, b.j) >= (std::min)(c.j, d.j))
			{
				return true;
			}
		}
		return false;
	}

	double rH = (a.i - c.i)*(d.j - c.j) - (a.j - c.j)*(d.i - c.i);
	double sH = (a.i - c.i)*(b.j - a.j) - (a.j - c.j)*(b.i - a.i);

	double r = rH / common;
	double s = sH / common;

	if (r >= 0 && r <= 1 && s >= 0 && s <= 1)
		return true;
	else
		return false;
}

bool BaseAvoidObst::IsPointBelogToLine(Cell a, Cell c, Cell d)
{
	return (a.i - c.i)*(d.j - c.j) - (a.j - c.j)*(d.i - c.i) == 0 &&
		(std::min)(c.i, d.i) <= a.i && (std::max)(c.i, d.i) >= a.i &&
		(std::min)(c.j, d.j) <= a.j && (std::max)(c.j, d.j) >= a.j;
}

std::vector<Cell> BaseAvoidObst::createAvoidPlan(std::vector<AONode>& AONodes)
{
	std::vector<Cell> plan;

	AONode lastAONode = AONodes[0];

	std::vector<Cell> nullTraj;

	brezenheim(start, lastAONode.cell, nullTraj);

	if (checkIntersectionNullTrajAndObstacles(nullTraj))
	{
		Cell cell(start_i, start_j);
		plan.push_back(cell);
	}

	plan.push_back(lastAONode.cell);

	AONodes.erase(AONodes.begin());
	Cell prevCell = lastAONode.cell;

	for (unsigned int i = 0; i < AONodes.size(); ++i)
	{
		if (AONodes[i].cell == prevCell)
		{
			continue;
		}

		plan.push_back(AONodes[i].cell);

		prevCell = AONodes[i].cell;
	}

	return plan;
}
