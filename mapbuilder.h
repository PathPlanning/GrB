#pragma once
#include"map.h"
#include"mapxmlreader.h"
#include<iostream>

class MapBuilder
{
private:
	const char* fileName;
protected:
public:
	MapBuilder(const char* fName)
	{
		fileName = fName;
	}

	~MapBuilder()
	{}

	map_ptr build()
	{
		MapXmlReader reader(fileName);

		map_ptr map = std::make_shared<Map>();

		int height;
		if (!reader.getHeight(height))
		{
			std::cout << "No 'height' element found in 'map' section." << std::endl;
			return nullptr;
		}
		map->height = height;

		int width;
		if (!reader.getWidth(width))
		{
			std::cout << "No 'width' element found in 'map' section." << std::endl;
			return nullptr;
		}
		map->width = width;

		int startx, starty;
		if (!reader.getStartPosition(startx, starty))
		{
			std::cout << "No 'startx' or 'starty' element found in 'map' section." << std::endl;
			return nullptr;
		}
		map->start = Cell(starty, startx);

		int goalx, goaly;
		if (!reader.getGoalPosition(goalx, goaly))
		{
			std::cout << "No 'goalx' or 'goaly' element found in 'map' section." << std::endl;
			return nullptr;
		}
		map->goal = Cell(goaly, goalx);

		int **grid = new int*[height];
		for (int i = 0; i < height; ++i) {
			grid[i] = new int[width];
		}

		if (!reader.getGrid(grid, height, width))
		{
			std::cout << "Problem with 'grid' data in 'map' section." << std::endl;
			return nullptr;
		}

		map->MTGraph = grid;

		return map;
	}
};
