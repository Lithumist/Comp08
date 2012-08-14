#include "fpscalc.h"

//         http://www.gamedev.net/page/resources/_/technical/game-programming/frame-rate-independent-movement-r1382
//		   with modifications

FpsCalc* FpsCalc::Instance = NULL;


FpsCalc* FpsCalc::GetInstance()
{
   if (!Instance)
      Instance = new FpsCalc;
 
   return Instance;
}

void FpsCalc::Init(double tfps)
{
  targetfps = tfps;
  QueryPerformanceCounter(&framedelay);
  QueryPerformanceFrequency(&tickspersecond);
}
void FpsCalc::SetSpeedFactor()
{
  QueryPerformanceCounter(&currentticks);
  //This frame's length out of desired length
  speedfactor = (double)(currentticks.QuadPart-framedelay.QuadPart)/((double)tickspersecond.QuadPart/targetfps);
  fps = targetfps/speedfactor;
  if (speedfactor <= 0)
	speedfactor = 1;

  framedelay = currentticks;
}

void FpsCalc::FreeMemory()
{
	if(Instance != NULL){ delete Instance; }
}