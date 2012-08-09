/*
	light.h

	Declares a function that lights up the level
*/

#ifndef LIGHT_H
#define LIGHT_H

#include <vector>

/// Actual definition inside light.h
/// This is to make a quick hack so that the light function can return a vector of mapCells
/// Yes there are other ways to do this but idgaf.

struct mapCell
{
	// If the cell is a normal walkable tile or not
	bool active;

	// The position of the cell IN TILE COORDINATES
	int x, y;

	// The type of cell
	// 01  wall
	int type;
};


// Lights the map

#endif