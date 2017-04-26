#ifndef GL_CONST_H
#define GL_CONST_H

//Metric Type
#define CN_MT_MANH 	0
#define CN_MT_DIAG 	1
#define CN_MT_EUCL	2

//Metric Type (in XML)
#define CNS_MANH "manhattan"
#define CNS_EUCL "euclid"
#define CNS_DIAG "diagonal"

//Number of parameter type
#define CN_NPT			4

//Parametrs Type
#define CN_PT_MT		0
#define CN_PT_NPP		1
#define CN_PT_MAXSTEPS	2
#define CN_PT_WEIGHT	3

//Obstacle
#define CN_OBSTL 1

//Movements Costs
static int CN_MC_STRAIGHT = 10;
static int CN_MC_DIAG = 14;

//logLevel type
#define CN_LOGLVL_NO        0
#define CN_LOGLVL_HIGH		1
#define CN_LOGLVL_LOW       2

#endif
