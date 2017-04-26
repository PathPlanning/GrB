#pragma once

#include<map>

#include"iavoidobst.h"
#include"map.h"
#include"ilogger.h"

//Course (Moving)
typedef enum CourseMoving
{
	CN_COUR_RIGHT = 10,
	CN_COUR_LEFT = 20,
	CN_COUR_UP = 30,
	CN_COUR_DOWN = 40,

	CN_COUR_NONE = 0
} CourseMoving;

//obstacle position
typedef enum ObstaclePosition
{
	CN_OBSTPOS_RIGHT = 1,
	CN_OBSTPOS_LEFT = 2,
	CN_OBSTPOS_UP = 3,
	CN_OBSTPOS_DOWN = 4,

	CN_OBSTPOS_NONE = 0
} obstaclePosition;

typedef enum RotationType
{
	RT_UP_RIGHT = CN_COUR_UP + CN_OBSTPOS_RIGHT,
	RT_LEFT_DOWN = CN_COUR_LEFT + CN_OBSTPOS_DOWN,

	RT_RIGHT_DOWN = CN_COUR_RIGHT + CN_OBSTPOS_DOWN,
	RT_UP_LEFT = CN_COUR_UP + CN_OBSTPOS_LEFT,

	RT_DOWN_LEFT = CN_COUR_DOWN + CN_OBSTPOS_LEFT,
	RT_RIGHT_UP = CN_COUR_RIGHT + CN_OBSTPOS_UP,

	RT_LEFT_UP = CN_COUR_LEFT + CN_OBSTPOS_UP,
	RT_DOWN_RIGHT = CN_COUR_DOWN + CN_OBSTPOS_RIGHT,

	RT_NONE = 0,

	RT_CAUTION = 100
} RotationType;

struct CoordOffset
{
	int iOffset, jOffset;

	CoordOffset(int i, int j) :iOffset(i), jOffset(j)
	{
	}

	CoordOffset() : iOffset(0), jOffset(0)
	{
	}

	CoordOffset(const CoordOffset& co) : iOffset(co.iOffset), jOffset(co.jOffset)
	{
	}
};

struct MovementData
{
	CourseMoving course;
	ObstaclePosition obstPos;

	MovementData(CourseMoving c, ObstaclePosition oP) : course(c), obstPos(oP)
	{
	}

	MovementData() : course(CN_COUR_NONE), obstPos(CN_OBSTPOS_NONE)
	{
	}

	MovementData(const MovementData& md) : course(md.course), obstPos(md.obstPos)
	{
	}
};

class AONode
{
public:
	Cell cell;
	RotationType rotate;
	ObstaclePosition obstPositionPrev, obstPositionNext;

	AONode()
	{
		rotate = RT_NONE;
	}

	AONode(int x, int y)
	{
		cell = Cell(x, y);
		rotate = RT_NONE;
	}

	AONode(int x, int y, RotationType r)
	{
		cell = Cell(x, y);
		rotate = r;
	}

	AONode(int x, int y, RotationType r, ObstaclePosition obstPrev, ObstaclePosition obstNext)
	{
		cell = Cell(x, y);
		rotate = r;
		obstPositionPrev = obstPrev;
		obstPositionNext = obstNext;
	}

	bool operator ==(AONode x)
	{
		return cell.i == x.cell.i && cell.j == x.cell.j && obstPositionNext == x.obstPositionNext && obstPositionPrev == x.obstPositionPrev;
	}

	bool operator !=(AONode x)
	{
		return cell.i != cell.i || cell.j != x.cell.j || obstPositionNext != x.obstPositionNext || obstPositionPrev != x.obstPositionPrev;
	}
};

class BaseAvoidObst : public IAvoidObst
{
protected:
	map_ptr map;
	logger_ptr logger;

	std::map<ObstaclePosition, CoordOffset> coordOffsetCheckObst;
	std::map<CourseMoving, CoordOffset> coordOffsetCurrentMovement;
	std::map<RotationType, MovementData> externalRotation;
	std::map<RotationType, MovementData> internalRotation;
	std::map<RotationType, RotationType> inverseRotation;

	CourseMoving startCourse, course;
	ObstaclePosition startObstPos, obstPosition;

	bool containsAllRotationPoint;

    int start_i, start_j;
    int cur_i, cur_j;
	Cell start, finish;

	CourseMoving HorizontalCourse;
	CourseMoving VerticalCourse;

	void getSectionMovingCourse(int start_i, int start_j, int finish_i, int finish_j);
	CourseMoving getHorizontalMovingCourse(int start_j, int finish_j);
	CourseMoving getVerticalMovingCourse(int start_i, int finish_i);
	void getStartClockwiseCourse();
	void getStartCounterClockwiseCourse();
	bool getAllRotationPoints(std::vector<AONode>& rotationPoints);
    bool addRotationPointIfPossible(std::vector<AONode>& rotationPoints, AONode newRotationPoint);

	std::vector<Cell> createAvoidPlan(std::vector<AONode>& AONodes);

    bool buildPathByDirectRotationPointList(std::vector<AONode>& rotationPoint, std::vector<AONode>& cornerAONodes, unsigned int startPosition);
    bool buildPathByDirectRotationWithHalfPointsList(std::vector<AONode>& rotationPoint, std::vector<AONode>& rPointsForCheck, std::vector<AONode>& cornerAONodes, unsigned int startPosition);
    bool buildPathByInverseRotationPointList(std::vector<AONode>& rotationPoint, std::vector<AONode>& cornerAONodes, int startPosition);

    bool checkIntersectionNullTrajAndCurrentObstacle(std::vector<Cell>& nullTraj, std::vector<AONode>& list, int& pos, AONode& curAONode);
	bool checkIntersectionNullTrajAndObstacles(std::vector<Cell>& nullTraj);
	int findPositionInListOfIntersectionWithSelectedAONode(std::vector<AONode>& list, Cell& start, Cell& finish, bool needToCheckObstPosition, AONode& curAONode);
	std::vector<ObstaclePosition> getPossibleObstPositions(Cell& start, Cell& finish);
	bool inline checkIsLinePartsIntersectedInCorrectDirection(Cell& cell1, Cell& cell2, Cell& start, Cell& finish, std::vector<ObstaclePosition>& possibleObstPosition, ObstaclePosition& curObstPosition);

	bool IsLinePartsIntersected(Cell a, Cell b, Cell c, Cell d);
	bool IsPointBelogToLine(Cell a, Cell c, Cell d);

public:
	BaseAvoidObst(map_ptr map, logger_ptr logger);
	~BaseAvoidObst();

	std::vector<std::vector<Cell>> getAvoidObstaclePlans(std::vector<Cell> nullTraj, int numberOfNodeOnTheObst, const Cell& start, const Cell& goal, unsigned int& maxNumberOfCreatedNode) override;
};
