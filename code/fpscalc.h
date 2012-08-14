#ifndef FPSCALC_H
#define FPSCALC_H

//         http://www.gamedev.net/page/resources/_/technical/game-programming/frame-rate-independent-movement-r1382
//		   with modifications

//			This is the only part of the game that is not cross platform
//			If you have any suggestions on removing the windows dependency, tell me


#include "windows.h"

class FpsCalc
{
public:
  double     	targetfps;
  double     	fps;
  LARGE_INTEGER tickspersecond;
  LARGE_INTEGER currentticks;
  LARGE_INTEGER framedelay;
    
  double     	speedfactor;   		 

  void      	Init(double tfps);
  void      	SetSpeedFactor();
  void			FreeMemory();

  static FpsCalc* GetInstance();
  static FpsCalc* Instance;
};

#endif