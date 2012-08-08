/*
	level.h

	Declares a class and structure that handles
	the levels in the game.
*/

#ifndef LEVEL_H
#define LEVEL_H

#include "global.h"
#include "ut.h"
#include <iostream>
#include <vector>

const int C_MAP_WIDTH_IN_TILES = 40;
const int C_MAP_HEIGHT_IN_TILES = 30;

const int C_MAP_TOTAL_SIZE = C_MAP_WIDTH_IN_TILES * C_MAP_HEIGHT_IN_TILES;

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

class level
{

	public:


		///
		/// Methods
		///

		// Constructor
		level();

		// Generates a new level
		void generate();

		// Draws the level
		void draw();


		// Index calculator for cell_data
		inline unsigned int getCellIndex(int x, int y)
		{
			return x * C_MAP_HEIGHT_IN_TILES + y;
		}

		// Returns the player start position (x) (only if level is ready)
		// Returns -1 if map isn't ready
		float getPlayerStartX();

		// Returns the player start position (y) (only if level is ready)
		// Returns -1 if map isn't ready
		float getPlayerStartY();

		// Completely recalculate the edgeList (goes through whole map!)
		void recalculateEdgeList();






		///
		/// Properties
		///


		// All the map cell data
		mapCell cell_data[C_MAP_WIDTH_IN_TILES * C_MAP_HEIGHT_IN_TILES]; // 30*40

		// If the map is ready to be updated and rendered
		bool map_is_ready;









	private:


		///
		/// Private properties
		///


		// Player start position x and y
		float p_start_x, p_start_y;

		// A vector of pointers to cells from "cell_data" that are edge walls
		std::vector<mapCell*> edgeList;


};

#endif