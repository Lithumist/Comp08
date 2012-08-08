/*
	level.h

	Defines a class and structure that handles
	the levels in the game.
*/

#include "level.h"





///
// Constructor
///
level::level()
{
	map_is_ready = false;
	p_start_x = 0;
	p_start_y = 0;
}

///
// Index calculator for cell_data
///
///
// In header file because of inline
///

/*
inline unsigned int getCellIndex(int x, int y)
{
	return x * C_MAP_HEIGHT_IN_TILES + y;
}
*/


///
// Returns the player start position (x) (only if level is ready)
// Returns -1 if map isn't ready
///
float level::getPlayerStartX(){ if(map_is_ready) return p_start_x; else return -1;}


///
// Returns the player start position (y) (only if level is ready)
// Returns -1 if map isn't ready
///
float level::getPlayerStartY(){ if(map_is_ready) return p_start_y; else return -1;}



///
// Generates a new level
///
void level::generate()
{
	map_is_ready = false;


	// fill map with walls
	for(int ypos=0; ypos<C_MAP_HEIGHT_IN_TILES; ypos++)
	{
		for(int xpos=0; xpos<C_MAP_WIDTH_IN_TILES; xpos++)
		{
			cell_data[getCellIndex(xpos,ypos)].active = true;
			cell_data[getCellIndex(xpos,ypos)].type = 1;
			cell_data[getCellIndex(xpos,ypos)].x = xpos;
			cell_data[getCellIndex(xpos,ypos)].y = ypos;
		}
	}

	// determine where the player will start and set it
	p_start_x = (float)ut::random(1,C_MAP_WIDTH_IN_TILES-2);	// -2 because function is inclusive
	p_start_y = (float)ut::random(1,C_MAP_HEIGHT_IN_TILES-2);	/////

	cell_data[getCellIndex(p_start_x,p_start_y)].active = false;
	cell_data[getCellIndex(p_start_x,p_start_y)].type = 0;


	map_is_ready = true;
}



///
// Draws the level
///
void level::draw()
{
	if(!map_is_ready)
		return;

	// brute force rendering
	for(int t=0; t<C_MAP_WIDTH_IN_TILES * C_MAP_HEIGHT_IN_TILES; t++)
	{
		if(cell_data[t].active && cell_data[t].type == 1)
		{
			float x, y;
			x = cell_data[t].x*16;
			y = cell_data[t].y*16;

			sf::RectangleShape rct;
			rct.setPosition(x,y);
			rct.setSize(sf::Vector2f(16,16));
			rct.setFillColor(sf::Color(100,100,100));

			global::rwpWindow->draw(rct);
		}
	}

}