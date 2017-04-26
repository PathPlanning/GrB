#pragma once

#include"ilogger.h"
#include"xmlwriter.h"

class BaseLogger : public ILogger, public XmlWriter
{
private:
protected:
public:
	BaseLogger(const char* fName);
	~BaseLogger();

	void save() override;
	void writeToLogSummary(SearchResult& result) override;
	void writeToLogPPlan(std::vector<Cell>& plan) override;
	void writeToLogPath(std::vector<Cell>& path) override;
	void writeToLogMap(map_ptr map, std::vector<Cell>& path) override;
	void writeToLogAllPaths(SearchResult& paths) override;
};