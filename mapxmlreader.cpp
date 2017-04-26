#include "mapxmlreader.h"
#include"utils.h"
#include<vector>
#include<iostream>

MapXmlReader::MapXmlReader(const char* fileName) : XmlReader(fileName)
{
	gotoFirstChildElement("root");
	gotoFirstChildElement("map");
}

MapXmlReader::~MapXmlReader()
{
}

bool MapXmlReader::getHeight(int& toValue)
{
	if (!gotoFirstChildElement("height"))
	{
		return false;
	}
	bool res = getIntValue(toValue);
	gotoParent();

	return res;
}

bool MapXmlReader::getWidth(int& toValue)
{
	if (!gotoFirstChildElement("width"))
	{
		return false;
	}
	
	bool res = getIntValue(toValue);
	gotoParent();

	return res;
}

bool MapXmlReader::getStartPosition(int& x, int& y)
{
	if (!gotoFirstChildElement("startx"))
		return false;

	bool res = getIntValue(x);
	if (!res)
	{
		gotoParent();
		return false;
	}

	gotoParent();
	
	if (!gotoFirstChildElement("starty"))
	{
		return false;
	}

	res = getIntValue(y);
	gotoParent();

	return res;
}

bool MapXmlReader::getGoalPosition(int& x, int& y)
{
	if (!gotoFirstChildElement("finishx"))
		return false;

	bool res = getIntValue(x);
	if (!res)
	{
		gotoParent();
		return false;
	}

	gotoParent();

	if (!gotoFirstChildElement("finishy"))
	{
		return false;
	}

	res = getIntValue(y);
	gotoParent();

	return res;
	std::vector<std::string> row;
}

bool MapXmlReader::getGrid(int** grid, unsigned int height, unsigned int width)
{
	std::vector<std::string> row;

	gotoFirstChildElement("grid");
	gotoFirstChildElement("row");
	for (unsigned int i = 0; i < height; ++i)
	{
		if(i != 0)
		{
			if (!gotoNextSiblingElement())
			{
				std::cout << "Not enough 'row' in 'grid' given" << std::endl;
				gotoParent();
				return false;
			}
		}

		std::string rowString;
		if (!getStringValue(rowString))
		{
			std::cout << "Problem with cells in 'row' " << i << "." << std::endl;
			gotoParent();
			return false;
		}

		row.clear();
		split(rowString, ' ', row);

		if (row.size() < width)
		{
			std::cout << "Not enough cells in 'row' " << i << " given." << std::endl;
			gotoParent();
			return false;
		}

		for (unsigned int j = 0; j < width; ++j)
		{
			grid[i][j] = std::stoi(row[j]);
		}
	}

	gotoParent();
	return true;
}