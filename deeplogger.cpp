#include"deeplogger.h"
#include<list>

DeepLogger::DeepLogger(const char* fName) : BaseLogger(fName)
{
}

DeepLogger::~DeepLogger()
{
}

void DeepLogger::writeToLogNodes(std::map<Cell, node_ptr, CellComparator>& open, std::map<Cell, node_ptr, CellComparator>& close)
{
	if (!gotoRoot())
	{
		return;
	}

	if (!gotoFirstChildElement("lowlevel"))
	{
		addNewElement("lowlevel");
		if (!gotoFirstChildElement("lowlevel"))
		{
			return;
		}
	}

	addNewElement("step");
	gotoLastChildElement("step");
	addAttribute("number", iteration);

	addNewElement("open");
	gotoFirstChildElement("open");
	writeNodes(open);
	gotoParent();

	addNewElement("close");
	gotoFirstChildElement("close");
	writeNodes(close);

	iteration++;
}

bool sortByF(const NodeInLogData& first, const NodeInLogData& second)
{
	return first.F < second.F;
}

void DeepLogger::writeNodes(std::map<Cell, node_ptr, CellComparator>& nodes)
{
	std::list<NodeInLogData> nodesInLog;

    for(auto nodeIter : nodes)
	{
		node_ptr node = nodeIter.second;

		NodeInLogData data;
		data.x = node->getCell().j;
		data.y = node->getCell().i;
		data.F = node->getF();
		data.g = node->getG();

		if (node->isParentListEmpty())
		{
			data.hasParent = false;
		}
		else
		{
			auto bestParent = node->getBestParent();
			if (bestParent == nullptr)
			{
				data.hasParent = false;
			}
			else
			{
				data.parent_x = bestParent->getCell().j;
				data.parent_y = bestParent->getCell().i;
				data.hasParent = true;
			}
		}
		nodesInLog.push_back(data);
	}

	nodesInLog.sort(sortByF);

    for(auto node : nodesInLog)
	{
		addNewElement("node");
		gotoLastChildElement("node");
		addAttribute("x", node.x);
		addAttribute("y", node.y);
		addAttribute("F", node.F);
		addAttribute("g", node.g);
		if(node.hasParent)
		{
			addAttribute("parent_x", node.parent_x);
			addAttribute("parent_y", node.parent_y);
		}

		gotoParent();
	}

	nodesInLog.clear();
}
