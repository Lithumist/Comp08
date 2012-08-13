/*
	level.h

	Declares a class and structure that handles
	the levels in the game.
*/

#ifndef LEVEL_H
#define LEVEL_H

#include "global.h"
#include "ut.h"
#include "light.h"
#include <iostream>
#include <vector>

const int C_MAP_WIDTH_IN_TILES = 40;
const int C_MAP_HEIGHT_IN_TILES = 30;

const int C_MAP_TOTAL_SIZE = C_MAP_WIDTH_IN_TILES * C_MAP_HEIGHT_IN_TILES;

/*

Actual definition inside light.h
This is to make a quick hack so that the light function can return a vector of mapCells
Yes there are other ways to do this but idgaf.

struct mapCell
{

};

*/

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

		// Re-lights the level by updating the drawList
		void doLight();

		// Updates the level
		void step();

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
		// Completely recalculate the walllList (goes through whole map!)
		void recalculateWallList();
		// Completely recalculate the walkableList (goes through whole map!)
		void recalculateWalkableList();






		///
		/// Properties
		///


		// All the map cell data
		mapCell cell_data[C_MAP_WIDTH_IN_TILES * C_MAP_HEIGHT_IN_TILES];

		// If the map is ready to be updated and rendered
		bool map_is_ready;

		// A pointer to the exit cell
		mapCell* exitCell;









	private:


		///
		/// Private properties
		///


		// Player start position x and y
		float p_start_x, p_start_y;

		// A vector of pointers to cells from "cell_data" that are edge walls
		std::vector<mapCell*> edgeList;
		// A vector of pointers to cells from "cell_data" that are walls
		std::vector<mapCell*> wallList;
		// A vector of pointers to cells from "cell_data" that are walkable
		std::vector<mapCell*> walkableList;

		// A vector of pointers to all the tiles that need to br drawn
		std::vector<mapCell*> drawList;


		sf::Sprite SPR_wall, SPR_u_wall, SPR_exit, SPR_lantern;






		///
		/// Private methods
		///

		void handleLine(int xx0, int yy0, int xx1, int yy1);


};

#endif