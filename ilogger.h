#pragma once
#include"searchresult.h"
#include"map.h"
#include<map>
#include"node.h"
#include<memory>

class ILogger
{
public:
	virtual void save() {}
	virtual void writeToLogSummary(SearchResult& result) {}
	virtual void writeToLogPPlan(std::vector<Cell>& plan) {}
	virtual void writeToLogPath(std::vector<Cell>& path) {}
	virtual void writeToLogMap(map_ptr map, std::vector<Cell>& path) {}
	virtual void writeToLogNodes(std::map<Cell, node_ptr, CellComparator>& open, std::map<Cell, node_ptr, CellComparator>& close) {}
	virtual void writeToLogAllPaths(SearchResult& paths) {}
};


class NullLogger : public ILogger
{
};

typedef std::shared_ptr<ILogger> logger_ptr;