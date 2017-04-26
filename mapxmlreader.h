#pragma once
#include"xmlreader.h"

class MapXmlReader : XmlReader
{
private:
protected:
public:
	MapXmlReader(const char* fileName);
	~MapXmlReader();

	bool getHeight(int& toValue);
	bool getWidth(int& toValue);

	bool getStartPosition(int& x, int& y);
	bool getGoalPosition(int& x, int& y);

	bool getGrid(int** grid, unsigned int height, unsigned int width);
};

