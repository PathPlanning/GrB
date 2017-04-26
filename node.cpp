#include"node.h"

double Parent::getTotalG()
{
	return G + data->getG();
}