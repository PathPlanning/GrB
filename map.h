#pragma once
#include<array>
#include<memory>
#include"cell.h"

class MapBuilder;

class Map 
{
private:
	int **MTGraph;
	int height, width;
	Cell start;
	Cell goal;

public:
	friend MapBuilder;
	Map()
	{
		MTGraph = nullptr;
	}
	~Map()
	{
		if(MTGraph != nullptr)
		{
			for (int i = 0; i<height; i++)
				delete[] MTGraph[i];
			delete[] MTGraph;
		}
	}

    inline unsigned int getHeight()
	{
		return height;
	}

    inline unsigned int getWidth()
	{
		return width;
	}

	inline int get(int i, int j)
	{
		if (i<0 || i >= height)
			return -1;

		if (j<0 || j >= width)
			return -1;

		return MTGraph[i][j];
	}

	inline Cell getStart()
	{
		return start;
	}

	inline Cell getGoal()
	{
		return goal;
	}
};

typedef std::shared_ptr<Map> map_ptr;

