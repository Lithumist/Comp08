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
	exitCell = NULL;
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
// Completely recalculate the edgeList (goes through whole map!)
///
void level::recalculateEdgeList()
{
	for(int ypos=1; ypos<C_MAP_HEIGHT_IN_TILES-1; ypos++)
	{
		for(int xpos=1; xpos<C_MAP_WIDTH_IN_TILES-1; xpos++)
		{ if(cell_data[getCellIndex(xpos,ypos)].type == 1) {

			if( cell_data[getCellIndex(xpos+1,ypos)].type == 0 ||
				cell_data[getCellIndex(xpos-1,ypos)].type == 0 ||
				cell_data[getCellIndex(xpos,ypos+1)].type == 0 ||
				cell_data[getCellIndex(xpos,ypos-1)].type == 0
				)

				edgeList.push_back(&cell_data[getCellIndex(xpos,ypos)]);

		}}
	}
}

///
// Completely recalculate the wallList (goes through whole map!)
///
void level::recalculateWallList()
{
	for(int ypos=1; ypos<C_MAP_HEIGHT_IN_TILES-1; ypos++)
	{
		for(int xpos=1; xpos<C_MAP_WIDTH_IN_TILES-1; xpos++)
		{

			if(cell_data[getCellIndex(xpos,ypos)].type == 1)
				wallList.push_back(&cell_data[getCellIndex(xpos,ypos)]);

		}
	}
}

///
// Completely recalculate the walkableList (goes through whole map!)
///
void level::recalculateWalkableList()
{
	for(int ypos=1; ypos<C_MAP_HEIGHT_IN_TILES-1; ypos++)
	{
		for(int xpos=1; xpos<C_MAP_WIDTH_IN_TILES-1; xpos++)
		{

			if(cell_data[getCellIndex(xpos,ypos)].type == 0)
				walkableList.push_back(&cell_data[getCellIndex(xpos,ypos)]);

		}
	}
}



///
// Generates a new level
///
void level::generate()
{
	map_is_ready = false;


	// Clear edge list
	edgeList.clear();


	// fill map with walls
	for(int ypos=0; ypos<C_MAP_HEIGHT_IN_TILES; ypos++)
	{
		for(int xpos=0; xpos<C_MAP_WIDTH_IN_TILES; xpos++)
		{
			if(xpos == 0 || xpos == C_MAP_WIDTH_IN_TILES-1 || ypos == 0 || ypos == C_MAP_HEIGHT_IN_TILES-1)
			{
				// add unbreakable wall
				cell_data[getCellIndex(xpos,ypos)].type = 2;
				cell_data[getCellIndex(xpos,ypos)].x = xpos;
				cell_data[getCellIndex(xpos,ypos)].y = ypos;
			}
			else
			{
				// add normal wall
				cell_data[getCellIndex(xpos,ypos)].type = 1;
				cell_data[getCellIndex(xpos,ypos)].x = xpos;
				cell_data[getCellIndex(xpos,ypos)].y = ypos;
			}
		}
	}

	// determine where the player will start and set it
	p_start_x = (float)ut::random(1,C_MAP_WIDTH_IN_TILES-3);	// -3 because function is inclusive
	p_start_y = (float)ut::random(1,C_MAP_HEIGHT_IN_TILES-3);	/////

	cell_data[getCellIndex(p_start_x,p_start_y)].type = 0;



	// Remove some random walls
	for(int t=0; t<10; t++)
	{
		int tx = ut::random(1,C_MAP_WIDTH_IN_TILES-3);	// -3 because function is inclusive
		int ty = ut::random(1,C_MAP_HEIGHT_IN_TILES-3);	/////

		if(cell_data[getCellIndex(tx,ty)].type != 2) // if it's not an unbreakable wall
			cell_data[getCellIndex(tx,ty)].type = 0;
	}



	recalculateEdgeList();


	// remove some walls from edge list
	for(int t=0; t<500; t++)
	{
		edgeList[ut::random(0,edgeList.size()-1)]->type = 0;
		recalculateEdgeList();
	}



	// At this point, the main level has been generated
	// All that needs to be done is to populate it with stuff

	// Add the exit
	recalculateWalkableList();

	exitCell = walkableList[ut::random(0,walkableList.size())];
	exitCell->type = 5;

	global::flExitCellX = (float)(exitCell->x*16);
	global::flExitCellY = (float)(exitCell->y*16);

	// debug
	std::cout << "Exit cell at " << exitCell->x << "," << exitCell->y << std::endl; 



	map_is_ready = true;
}



///
// Draws the level
///
void level::draw()
{


	if(!map_is_ready)
		return;


	// Draw cells
	for(int t=0; t<drawList.size(); t++)
	{
		if(drawList[t]->type == 1) // wall
		{
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			sf::RectangleShape rct;
			rct.setPosition(xpos,ypos);
			rct.setSize(sf::Vector2f(16,16));
			rct.setFillColor(sf::Color(100,100,100));

			global::rwpWindow->draw(rct);
		}

		else if(drawList[t]->type == 0) // walkable
		{
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			sf::RectangleShape rct;
			rct.setPosition(xpos,ypos);
			rct.setSize(sf::Vector2f(16,16));
			rct.setFillColor(sf::Color(200,200,200));

			global::rwpWindow->draw(rct);
		}

		else if(drawList[t]->type == 2) // unbreakable wall
		{
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			sf::RectangleShape rct;
			rct.setPosition(xpos,ypos);
			rct.setSize(sf::Vector2f(16,16));
			rct.setFillColor(sf::Color(0,0,150));

			global::rwpWindow->draw(rct);
		}

		else if(drawList[t]->type == 3) //lantern
		{
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			sf::RectangleShape rct;
			rct.setPosition(xpos,ypos);
			rct.setSize(sf::Vector2f(16,16));
			rct.setFillColor(sf::Color(255,255,0));

			global::rwpWindow->draw(rct);
		}

		else if(drawList[t]->type == 5) // exit
		{
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			sf::RectangleShape rct;
			rct.setPosition(xpos,ypos);
			rct.setSize(sf::Vector2f(16,16));
			rct.setFillColor(sf::Color(0,255,255));

			global::rwpWindow->draw(rct);
		}
	}



}






///
// Updates the level
///
void level::step()
{

	// Calculate light
	doLight();

}








///
// Re-lights the level by updateing the drawList
// No loops to lessen the complexity
///
void level::doLight()
{
	// clear the draw list
	drawList.clear();

	
	// Calculate player tile x and y
	int player_tile_x, player_tile_y;
	player_tile_x = (int)(ut::round(*global::flPlayerX/16));
	player_tile_y = (int)(ut::round(*global::flPlayerY/16));


	// Handle lines in straight directions
	handleLine(player_tile_x,player_tile_y,player_tile_x,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y);




	// Handle lines in the up+right direction
	handleLine(player_tile_x,player_tile_y,player_tile_x+1,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+2,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+3,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+4,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+5,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+6,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+7,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+8,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+9,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+10,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+11,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+12,player_tile_y-C_PLAYER_LIGHT_DISTANCE);

	// Handle lines in the up+left direction
	handleLine(player_tile_x,player_tile_y,player_tile_x-1,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-2,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-3,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-4,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-5,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-6,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-7,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-8,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-9,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-10,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-11,player_tile_y-C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-12,player_tile_y-C_PLAYER_LIGHT_DISTANCE);





	// Handle lines in the down+left direction
	handleLine(player_tile_x,player_tile_y,player_tile_x-1,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-2,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-3,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-4,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-5,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-6,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-7,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-8,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-9,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-10,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-11,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x-12,player_tile_y+C_PLAYER_LIGHT_DISTANCE);

	// Handle lines in the down+right direction
	handleLine(player_tile_x,player_tile_y,player_tile_x+1,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+2,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+3,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+4,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+5,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+6,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+7,player_tile_y+C_PLAYER_LIGHT_DISTANCE);            
	handleLine(player_tile_x,player_tile_y,player_tile_x+8,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+9,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+10,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+11,player_tile_y+C_PLAYER_LIGHT_DISTANCE);
	handleLine(player_tile_x,player_tile_y,player_tile_x+12,player_tile_y+C_PLAYER_LIGHT_DISTANCE);






	// Handle lines in the right+up direction
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-1);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-2);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-3);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-4);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-5);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-6);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-7);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-8);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-9);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-10);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y-11);

	// Handle lines in the right+down direction
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+1);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+2);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+3);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+4);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+5);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+6);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+7);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+8);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+9);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+10);
	handleLine(player_tile_x,player_tile_y,player_tile_x+C_PLAYER_LIGHT_DISTANCE,player_tile_y+11);






	// Handle lines in the left+down direction
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+1);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+2);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+3);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+4);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+5);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+6);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+7);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+8);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+9);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+10);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y+11);

	// Handle lines in the left+up direction
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-1);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-2);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-3);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-4);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-5);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-6);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-7);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-8);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-9);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-10);
	handleLine(player_tile_x,player_tile_y,player_tile_x-C_PLAYER_LIGHT_DISTANCE,player_tile_y-11);
}















void level::handleLine(int xx0, int yy0, int xx1, int yy1)
{
	std::vector<sf::Vector2i> tmp = ut::calculateLine(xx0,yy0,xx1,yy1);
	for(int t=0; t<tmp.size(); t++)
	{
		// For each cell in the line
		// Add it to the draw list and stop if it's a block

		drawList.push_back(&cell_data[getCellIndex(tmp[t].x,tmp[t].y)]);

		if(cell_data[getCellIndex(tmp[t].x,tmp[t].y)].type == 1 || cell_data[getCellIndex(tmp[t].x,tmp[t].y)].type == 2)
			break;
	}
}