/*
	light.h

	Declares a function that lights up the level
*/

#ifndef LIGHT_H
#define LIGHT_H

#include <vector>



// DO NOT CHANGE THESE!!!!!!!!!!!!!!!!!
const int C_PLAYER_LIGHT_DISTANCE = 12;
const int C_LANTERN_LIGHT_DISTANCE = 8;




/// Actual definition inside light.h
/// This is to make a quick hack so that the light function can return a vector of mapCells
/// Yes there are other ways to do this but idgaf.

struct mapCell
{


	// The position of the cell IN TILE COORDINATES
	int x, y;

	// The type of cell
	// 00  walkable
	// 01  wall
	// 02  unbreakable wall
	// 03  lantern
	// 04  chest
	// 05  exit
	int type;


};


// Lights the map

#endif