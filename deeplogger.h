#pragma once
#include"baselogger.h"
class DeepLogger : public BaseLogger
{
private:
	unsigned int iteration = 0;
	void writeNodes(std::map<Cell, node_ptr, CellComparator>& nodes);
public:
	DeepLogger(const char* fName);
	~DeepLogger();

	void writeToLogNodes(std::map<Cell, node_ptr, CellComparator>& open, std::map<Cell, node_ptr, CellComparator>& close) override;
};

struct NodeInLogData
{
	int x, y;
	double F, g;
	int parent_x, parent_y;
	bool hasParent;
};