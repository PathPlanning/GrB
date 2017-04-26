#include"baselogger.h"
#include<string>
#include<sstream>
#include<iostream>
#include"utils.h"

BaseLogger::BaseLogger(const char* fName) : XmlWriter(fName)
{
	/*auto root = pDoc->first_node("root");
	logElement = std::unique_ptr<rapidxml::xml_node<>>(pDoc->allocate_node(rapidxml::node_element, "log"));
	root->append_node(logElement.get());

	logElement->append_node(pDoc->allocate_node(rapidxml::node_element, "mapfilename", fileName.c_str()));
	logElement->append_node(pDoc->allocate_node(rapidxml::node_element, "summary"));
	logElement->append_node(pDoc->allocate_node(rapidxml::node_element, "path")); */

	if(gotoFirstChildElement("root"))
	{ 
		if (addNewElement("log"))
		{
			gotoFirstChildElement("log");
			setCurrentElementAsRoot();
		}
	}
	else
	{
		return;
	}

	gotoRoot();
	addNewElement("mapfilename", getFileName());
	addNewElement("summary");
	addNewElement("pathonmap");
}

BaseLogger::~BaseLogger()
{
}



void BaseLogger::writeToLogSummary(SearchResult& result)
{
	if (!gotoRoot())
	{
		return;
	}

	if (!gotoFirstChildElement("summary"))
	{
		addNewElement("summary");
		gotoFirstChildElement("summary");
	}

	/*addAttribute("pathfound", result.pathfound);
	addAttribute("numberofsteps", result.numberofsteps);
	addAttribute("sectionscreated", result.sectionscreated);
	addAttribute("peaksectionscreated", result.peaksectionscreated);
	addAttribute("pathlength", result.pathlength);*/
	addAttribute("pathsfound", result.pathsFound);
	addAttribute("numberofsteps", result.numberofsteps);
	addAttribute("sectionscreated", result.sectionscreated);
	addAttribute("peaksectionscreated", result.peaksectionscreated);
	addAttribute("sumlength", result.sumLength);
	if (!result.paths.empty())
	{
		double avgLength = result.sumLength / result.paths.size();
		addAttribute("avglength", avgLength);
	}
	addAttribute("time", result.time);
}

void BaseLogger::writeToLogPPlan(std::vector<Cell>& plan)
{
	if (!gotoRoot())
	{
		return;
	}

	if (plan.empty())
	{
		return;
	}

	addNewElement("hplevel");
	gotoFirstChildElement("hplevel");

	for (unsigned int i = 1; i < plan.size(); ++i)
	{
		addNewElement("section");
		if (gotoLastChildElement("section"))
		{
			Cell startCell = plan[i - 1];
			Cell finishCell = plan[i];
			double length = calculateDistance(startCell, finishCell);

			addAttribute("number", i);
			addAttribute("start.x", startCell.j);
			addAttribute("start.y", startCell.i);
			addAttribute("finish.x", finishCell.j);
			addAttribute("finish.y", finishCell.i);
			addAttribute("length", length);
			gotoParent();
		}
	}
}

void BaseLogger::writeToLogPath(std::vector<Cell>& path)
{
	if (!gotoRoot())
	{
		return;
	}

	addNewElement("lplevel");
	gotoFirstChildElement("lplevel");

	if (path.empty())
	{
		addNewElement("nopathfound");
		return;
	}

	for (unsigned int i = 0; i < path.size(); ++i)
	{
		addNewElement("node");
		gotoLastChildElement("node");

		addAttribute("number", i);
		addAttribute("x", path[i].j);
		addAttribute("y", path[i].i);

		gotoParent();
	}
}

void BaseLogger::writeToLogMap(map_ptr map, std::vector<Cell>& path)
{

	if (!gotoRoot())
	{
		return;
	}

	if (map == nullptr)
	{
		return;
	}

	
	if (!gotoFirstChildElement("pathonmap"))
	{
		addNewElement("pathonmap");
		gotoFirstChildElement("pathonmap");
	}

	int* curLine = new int[map->getWidth()];
	for (int i = 0; i < map->getWidth(); ++i)
		curLine[i] = 0;

	for (unsigned int i = 0; i < map->getHeight(); ++i)
	{
		std::string value;
		value.clear();

		for (unsigned int k = 0; k < path.size(); k++)
		{
			if (path[k].i == i)
				curLine[path[k].j] = 1;

		}

		for (int j = 0; j < map->getWidth(); j++)
		{
			if (curLine[j] != 1)
				value = value.append(std::to_string(map->get(i, j)) + " ");
			else
			{
				value = value.append("* ");
				curLine[j] = 0;
			}
		}

		addNewElement("row", value);
		gotoLastChildElement("row");
		addAttribute("number", i);
		gotoParent();
	}

	delete curLine;
}

void BaseLogger::writeToLogAllPaths(SearchResult& result)
{
	std::vector<std::vector<Cell>> paths = result.paths;
	result.sumLength = 0;

	if (paths.empty())
	{
		return;
	}

	if (!gotoRoot())
	{
		return;
	}

	for (unsigned int i = 0; i < paths.size(); ++i)
	{
		bool pathFound = true;

		addNewElement("path");
		gotoLastChildElement("path");
		addAttribute("number", i);
		addAttribute("pathfound", pathFound);

		auto plan = paths[i];

		double pathLength = 0;

		addNewElement("hplevel");
		gotoFirstChildElement("hplevel");

		for(unsigned int j = 1; j < plan.size(); ++j)
		{	
			addNewElement("section");
			if (gotoLastChildElement("section"))
			{
				Cell startCell = plan[j - 1];
				Cell finishCell = plan[j];
				double length = calculateDistance(startCell, finishCell);
				pathLength += length;
				addAttribute("number", j);
				addAttribute("start.x", startCell.j);
				addAttribute("start.y", startCell.i);
				addAttribute("finish.x", finishCell.j);
				addAttribute("finish.y", finishCell.i);
				addAttribute("length", length);
				gotoParent();
			}
		}

		gotoParent();
		addAttribute("length", pathLength);
		result.sumLength += pathLength;
		gotoParent();
	}
}

void BaseLogger::save()
{
	XmlWriter::save();
}