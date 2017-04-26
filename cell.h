#pragma once

class Cell
{
public:
    int i;
    int j;

	Cell()
	{
		i = -1;
		j = -1;
	}

    Cell(int x, int y)
	{
		i = x;
		j = y;
	}

	bool operator==(Cell &p)
	{
		return i == p.i && j == p.j;
	}
};

struct CellComparator {
	bool operator()(const Cell& a, const Cell& b) const 
	{
		if (a.i < b.i)
			return true;
		else if (a.i == b.i)
			return a.j < b.j;
		return false;
	}
};
