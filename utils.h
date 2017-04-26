#pragma once

#include<vector>
#include<regex>
#include"cell.h"
#include"gl_const.h"

#ifdef __linux__
#include <sys/time.h>
#else
#include <windows.h>
#endif

//std::vector<std::string> split(const std::string& input, const std::string& regex);
void split(const std::string& s, char c, std::vector<std::string>& v);

static int METRICTYPE;
static double WEIGHT = 1;

double calculateDistance(Cell from, Cell to);

double calculateH(Cell from, Cell to);

void brezenheim(const Cell& cell1, const Cell& cell2, std::vector<Cell>& nullTraj);

#ifdef __linux__
static timeval beginTimer, endTimer;

#else
static LARGE_INTEGER beginTimer, endTimer, freqTimer;
#endif

void startTimer();
void stopTimer();
double getTime();