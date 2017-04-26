#include"utils.h"


double calculateDistance(Cell from, Cell to)
{
	int delta_i, delta_j;
	delta_i = abs(to.i - from.i);
	delta_j = abs(to.j - from.j);

	double length;

	switch (METRICTYPE)
	{
	case CN_MT_MANH:
		length = float(CN_MC_STRAIGHT*(delta_i + delta_j));
		break;

	case CN_MT_DIAG:
		if (delta_i > delta_j)
			length = float(CN_MC_DIAG * delta_j + CN_MC_STRAIGHT * (delta_i - delta_j));
		else
			length = float(CN_MC_DIAG * delta_i + CN_MC_STRAIGHT * (delta_j - delta_i));
		break;

	case CN_MT_EUCL:
		length = CN_MC_STRAIGHT * sqrt(float(delta_i * delta_i) + float(delta_j * delta_j));
		break;
	}

	return length;
}

double calculateH(Cell from, Cell to)
{
	return calculateDistance(from, to) * WEIGHT;
}

/*
std::vector<std::string> split(const std::string& input, const std::string& regex) {
	std::regex re(regex);
	std::sregex_token_iterator
		first{ begin(input), end(input), re, -1 },
		last;
	return{ first, last };
}
*/

void split(const std::string& s, char c, std::vector<std::string>& v) {
	std::string::size_type i = 0;
	std::string::size_type j = s.find(c);

	while (j != std::string::npos) {
		v.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(c, j);

		if (j == std::string::npos)
			v.push_back(s.substr(i, s.length()));
	}
}

void brezenheim(const Cell& cell1, const Cell& cell2, std::vector<Cell>& nullTraj)
{

	int x1 = cell1.i;
	int x2 = cell2.i;
	int y1 = cell1.j;
	int y2 = cell2.j;

	int xSpacing = std::abs(x1 - x2);
	int ySpacing = std::abs(y1 - y2);

	int len = (std::max)(xSpacing, ySpacing) + 1;
	nullTraj.resize(len);
	int index = 0;

	int x, y;
	int dx, dy;
	int StepVal = 0;
	int Rotate = 0;
	int Tmp;
	Cell newNode;

	if (x1 > x2 && y1 > y2)
	{
		Tmp = x2;
		x2 = x1;
		x1 = Tmp;

		Tmp = y2;
		y2 = y1;
		y1 = Tmp;

		dx = x2 - x1;
		dy = y2 - y1;
	}
	else
	{
		dx = x2 - x1;
		dy = y2 - y1;

		if (dx >= 0 && dy >= 0)
		{
			Rotate = 2;
		}
		else if (dy < 0)
		{
			dy = -dy;

			Tmp = y2;
			y2 = y1;
			y1 = Tmp;

			Rotate = 1;
		}
		else if (dx < 0)
		{
			dx = -dx;

			Tmp = x2;
			x2 = x1;
			x1 = Tmp;

			Rotate = 3;
		}
	}

	if (Rotate == 1)
	{
		if (dx >= dy)
		{
			y = y2;

			index = 0;

			for (x = x1; x <= x2; x++)
			{
				newNode.i = x;
				newNode.j = y;
				//nullTraj.push_back(newNode);
				nullTraj[index] = newNode;
				index++;

				StepVal += dy;
				if (StepVal >= dx)
				{
					y--;
					StepVal -= dx;
				}
			}
		}
		else
		{
			x = x2;
			index = nullTraj.size() - 1;

			for (y = y1; y <= y2; y++)
			{
				newNode.i = x;
				newNode.j = y;
				//nullTraj.insert(nullTraj.begin(),newNode);
				nullTraj[index] = newNode;
				index--;

				StepVal += dx;
				if (StepVal >= dy)
				{
					x--;
					StepVal -= dy;
				}
			}
		}

		return;
	}
	else if (Rotate == 2)
	{
		if (dx >= dy)
		{
			y = y1;
			index = 0;

			for (x = x1; x <= x2; x++)
			{
				newNode.i = x;
				newNode.j = y;
				//nullTraj.push_back(newNode);
				nullTraj[index] = newNode;
				index++;

				StepVal += dy;
				if (StepVal >= dx)
				{
					y++;
					StepVal -= dx;
				}
			}

			return;
		}
		else
		{
			x = x1;
			index = 0;
			for (y = y1; y <= y2; y++)
			{
				newNode.i = x;
				newNode.j = y;
				//nullTraj.push_back(newNode);
				nullTraj[index] = newNode;
				index++;

				StepVal += dx;
				if (StepVal >= dy)
				{
					x++;
					StepVal -= dy;
				}
			}

			return;
		}
	}
	else if (Rotate == 3)
	{
		if (dx >= dy)
		{
			y = y2;
			index = nullTraj.size() - 1;
			for (x = x1; x <= x2; x++)
			{
				newNode.i = x;
				newNode.j = y;
				//nullTraj.insert(nullTraj.begin(),newNode);
				nullTraj[index] = newNode;
				index--;

				StepVal += dy;
				if (StepVal >= dx)
				{
					y--;
					StepVal -= dx;
				}
			}
		}
		else
		{
			x = x2;
			index = 0;

			for (y = y1; y <= y2; y++)
			{
				newNode.i = x;
				newNode.j = y;
				//nullTraj.push_back(newNode);
				nullTraj[index] = newNode;
				index++;

				StepVal += dx;
				if (StepVal >= dy)
				{
					x--;
					StepVal -= dy;
				}
			}
		}
		return;
	}

	if (dx >= dy)
	{
		y = y1;
		index = nullTraj.size() - 1;

		for (x = x1; x <= x2; x++)
		{
			newNode.i = x;
			newNode.j = y;
			//nullTraj.insert(nullTraj.begin(),newNode);
			nullTraj[index] = newNode;
			index--;

			StepVal += dy;

			if (StepVal >= dx)
			{
				y++;
				StepVal -= dx;
			}
		}
	}
	else
	{
		x = x1;
		index = nullTraj.size() - 1;

		for (y = y1; y <= y2; y++)
		{
			newNode.i = x;
			newNode.j = y;
			//nullTraj.insert(nullTraj.begin(),newNode);
			nullTraj[index] = newNode;
			index--;

			StepVal += dx;
			if (StepVal >= dy)
			{
				x++;
				StepVal -= dy;
			}
		}
	}
}

void startTimer()
{
#ifdef __linux__
	gettimeofday(&beginTimer, NULL);
#else
	QueryPerformanceCounter(&beginTimer);
	QueryPerformanceFrequency(&freqTimer);
#endif
}

void stopTimer()
{
#ifdef __linux__
	gettimeofday(&endTimer, NULL);

#else
	QueryPerformanceCounter(&endTimer);
#endif
}

double getTime()
{
	double res;
#ifdef __linux__
	res = (endTimer.tv_sec - beginTimer.tv_sec) + static_cast<double>(endTimer.tv_usec - beginTimer.tv_usec) / 1000000;
#else
	res = static_cast<double>(endTimer.QuadPart - beginTimer.QuadPart) / freqTimer.QuadPart;
#endif

	return res;
}