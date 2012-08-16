/*
	light.h

	Does nothing useful apart from make the mapCell struct usable by many other files without having to include level.h
*/

#ifndef LIGHT_H
#define LIGHT_H

#include <vector>



// DO NOT CHANGE THESE!!!!!!!!!!!!!!!!!
const int C_PLAYER_LIGHT_DISTANCE = 12;
const int C_LANTERN_LIGHT_DISTANCE = 8;




// Originaly, there was going to be a light.cpp and the light calculation function. But I put it in the level files
// instead. This remains.

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


	// This only applies if the cell is a chest
	// 
	// If the chest has been looted or not
	bool looted;



	// This only applies if the cell is a chest
	// 
	// How much treasure the player will gain from looting the chest
	int treasure;



	// This only applies if the cell is a chest
	// 
	// how many lanterns the player will get from looting the chest
	int lanterns;


};


// Lights the map

#endif