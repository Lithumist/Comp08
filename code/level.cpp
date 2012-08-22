/*
	level.cpp

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
	whichHalf = false;
	hf_1 = 0;
	hf_2 = 0;
	count = 0;
	zombieCount = 0;

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
	edgeList.clear();
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
	walkableList.clear();
	for(int ypos=0; ypos<C_MAP_HEIGHT_IN_TILES; ypos++)
	{
		for(int xpos=0; xpos<C_MAP_WIDTH_IN_TILES; xpos++)
		{

			if(cell_data[getCellIndex(xpos,ypos)].type == 0)
				walkableList.push_back(&cell_data[getCellIndex(xpos,ypos)]);

		}
	}
}


///
// Completely recalculate the spawnList (goes through whole of walkableList)
///
void level::recalculateSpawnList()
{
	recalculateWalkableList();
	spawnList.clear();

	// Add all walkable cells
	for(int t=0; t<walkableList.size(); t++)
	{
		if(!walkableList[t]->lit)
			spawnList.push_back(walkableList[t]);
	}

}


///
// Init
///
void level::init()
{
	SPR_chest_closed.setTexture(global::TXT_chest_closed);
	SPR_chest_open.setTexture(global::TXT_chest_open);
	SPR_wall.setTexture(global::TXT_wall);
	SPR_u_wall.setTexture(global::TXT_u_wall);
	SPR_exit.setTexture(global::TXT_exit);
	SPR_lantern.setTexture(global::TXT_lantern);


	zombieList.clear();
}



///
// Generates a new level
///
void level::generate()
{

	map_is_ready = false;



	// Clear edge list
	edgeList.clear();

	// Clear lantern list
	lanternList.clear();

	// Clear zombie list
	zombieList.clear();
	zombieCount = 0;
	maxZombies = *global::ptrCurLevel;

	// Reset spawn clock
	spawnClock.restart();


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

	std::cout << "player at " << p_start_x << "," << p_start_y << std::endl;




	// Remove some random walls
	for(int t=0; t<5; t++)
	{
		int tx = ut::random(1,C_MAP_WIDTH_IN_TILES-3);	// -3 because function is inclusive
		int ty = ut::random(1,C_MAP_HEIGHT_IN_TILES-3);	/////

		if(cell_data[getCellIndex(tx,ty)].type != 2) // if it's not an unbreakable wall
			cell_data[getCellIndex(tx,ty)].type = 0;
	}



	recalculateEdgeList();


	// remove some walls from edge list
	if(edgeList.size() != 0)
	{
		for(int t=0; t<150; t++)
		{
			edgeList[ut::random(0,edgeList.size()-1)]->type = 0;
			recalculateEdgeList();
		}
	}



	// At this point, the main level has been generated
	// All that needs to be done is to populate it with stuff

	// Add the exit
	bool addedExit = false;
	int count = 0;
	while(!addedExit)
	{


		count ++;
		exitCell = edgeList[ut::random(0,edgeList.size()-2)];
		if((exitCell->x >= p_start_x+13 || exitCell->x <= p_start_x-13) && (exitCell->y >= p_start_y+13 || exitCell->y <= p_start_y-13))
			addedExit = true;
		else
		{
			if(count > 100)
			{
				// If there have been over 100 failed attempts,
				// there's a good chance that it's impossible to place the exit where we want to.
				// We need to place the exit somewhere surrounded by walls
				bool addedExit2 = false;
				float e_start_x, e_start_y;
				while(!addedExit2)
				{

					e_start_x = (float)ut::random(1,C_MAP_WIDTH_IN_TILES-3);
					e_start_y = (float)ut::random(1,C_MAP_HEIGHT_IN_TILES-3);

					exitCell = &cell_data[getCellIndex(e_start_x,e_start_y)];
					if((exitCell->x >= p_start_x+13 || exitCell->x <= p_start_x-13) && (exitCell->y >= p_start_y+13 || exitCell->y <= p_start_y-13))
					{
						addedExit2 = true;
						addedExit = true;
					}
				}

			}
		}


	}

	exitCell->type = 5;

	global::flExitCellX = (float)(exitCell->x*16);
	global::flExitCellY = (float)(exitCell->y*16);

	// debug
	//std::cout << "Exit cell at " << exitCell->x << "," << exitCell->y << std::endl; 



	// Add the chests

	chestList.clear();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	const int NUMBER_OF_CHESTS = 2;
	int rnd[NUMBER_OF_CHESTS];
	bool addedChest, addedChest2;
	int chestCount;
	int id = 0;
	bool isEdge[NUMBER_OF_CHESTS];

	for(int t=0; t<NUMBER_OF_CHESTS; t++)
	{
		std::cout << "adding chest " << t << std::endl;
		addedChest = false;
		chestCount = 0;

		while(!addedChest)
		{
			chestCount ++;
			id = ut::random(0,edgeList.size()-2);
			if(id >= (int)edgeList.size())
				id --;

			if((edgeList[id]->x >= p_start_x+10 || edgeList[id]->x <= p_start_x-10) && (edgeList[id]->y >= p_start_y+10 || edgeList[id]->y <= p_start_y-10))
			{
				std::cout << "success\n";
				addedChest = true;
				chestList.push_back(edgeList[id]);///
				isEdge[t] = true;
				rnd[t] = id;
			}
			else
			{
				if(chestCount > 100)
				{
					std::cout << "fallback\n";
					addedChest2 = false;
					float cx,cy;
					mapCell* chestCell = NULL;
					while(!addedChest2)
					{
						cx = (float)ut::random(1,C_MAP_WIDTH_IN_TILES-3);
						cy = (float)ut::random(1,C_MAP_HEIGHT_IN_TILES-3);

						chestCell = &cell_data[getCellIndex(cx,cy)];
						if((exitCell->x >= p_start_x+10 || exitCell->x <= p_start_x-10) && (exitCell->y >= p_start_y+10 || exitCell->y <= p_start_y-10))
						{
							addedChest2 = true;
							addedChest = true;
							chestList.push_back(chestCell);///
							std::cout << "yea\n";
							isEdge[t] = false;
							rnd[t] = getCellIndex(cx,cy);
						}


					}//while


				}//if


			}//else


		}//while


	}//for





	for(int t=0; t<NUMBER_OF_CHESTS; t++)
	{
		recalculateEdgeList();

		mapCell* working = NULL;
		if(isEdge[t])
		{
			if(rnd[t] >= (int)edgeList.size())
				rnd[t] -= 1;
			working = edgeList[rnd[t]];
		}
		else
			working = &cell_data[rnd[t]];

		working->type = 4;
		working->looted = false;
		working->treasure = ut::random(1,6);
		int firstRnd = ut::random(1,4);
		if(firstRnd == 1)
		{
			// 25% chance of ending up here
			// when here there is also a 25% chance of getting no lanterns
			// This all means that there is a 87.5% chance of getting no lanterns from this chest
			working->lanterns = ut::random(0,3);
		}
		else
		{
			// 75% chance of ending up here
			working->lanterns = 0;
		}

		//std::cout << "Chest " << t << " at " << edgeList[t]->x << "," << edgeList[t]->y << std::endl; 
	}



	map_is_ready = true;
}



///
// Draws the level
///
void level::draw()
{


	if(!map_is_ready)
		return;

	int cur_it = 0;
	sf::RectangleShape rct;



	// Draw cells
	for(int t=0; t<(int)drawList.size(); t++)
	{
		if(drawList[t]->type == 1) // wall
		{
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			SPR_wall.setPosition(xpos,ypos);

			global::rwpWindow->draw(SPR_wall);
		}

		else if(drawList[t]->type == 0) // walkable
		{
			cur_it = t;
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			rct.setPosition(xpos,ypos);
			rct.setSize(sf::Vector2f(16,16));
			rct.setFillColor(sf::Color(135,124,90));

			global::rwpWindow->draw(rct);

			// Draw zombie if needed
			if(drawList[t]->isZombie)
			{
				goto draw_zomb;
			}
done_draw_zomb:

			cur_it = cur_it; // this line means and does nothing important
					   // it is just here so that the label doesnt trigger a compiler error

		}

		else if(drawList[t]->type == 2) // unbreakable wall
		{
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			SPR_u_wall.setPosition(xpos,ypos);

			global::rwpWindow->draw(SPR_u_wall);
		}

		else if(drawList[t]->type == 3) //lantern
		{
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			SPR_lantern.setPosition(xpos,ypos);

			global::rwpWindow->draw(SPR_lantern);
		}

		else if(drawList[t]->type == 4) // chest
		{
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			if(drawList[t]->looted)
			{
				SPR_chest_open.setPosition(xpos,ypos);
				global::rwpWindow->draw(SPR_chest_open);
			}
			else
			{
				SPR_chest_closed.setPosition(xpos,ypos);
				global::rwpWindow->draw(SPR_chest_closed);
			}
		}

		else if(drawList[t]->type == 5) // exit
		{
			float xpos, ypos;
			xpos = drawList[t]->x*16;
			ypos = drawList[t]->y*16;

			SPR_exit.setPosition(xpos,ypos);

			global::rwpWindow->draw(SPR_exit);
		}
	}





		return;




	
	//////////////////////////////////
	// Draw zombies
draw_zomb:

	drawList[cur_it]->ptrZombie->draw();

	goto done_draw_zomb;


}






///
// Updates the level
///
void level::step()
{

	// Calculate light
	doLight();



	// Spawn zombies every 30 seconds if needed
	if(spawnClock.getElapsedTime().asSeconds() >= C_SPAWN_CLOCK)
	{
		if(zombieCount < maxZombies){

		zombieCount ++;

		std::cout << "Spawning zombie at ";

		zombie new_zombie;
		
		// Select random location
		recalculateSpawnList();
		int rndCell = ut::random(0,(int)spawnList.size()-1);



		float xp,yp;
		xp = spawnList[rndCell]->x*16;
		yp = spawnList[rndCell]->y*16;

		std::cout << xp/16 << "," << yp/16 << std::endl;


		// Init new zombie
		if(*global::ptrCurLevel == 1)
			zombie_hp = 1;
		else
			zombie_hp = (int)(*global::ptrCurLevel/2);
		
		zombie_damage = 2 * (*global::ptrCurLevel);


		new_zombie.reset(xp,yp,zombie_hp,zombie_damage);

		// Add zombie to list
		zombieList.push_back(new_zombie);
		}spawnClock.restart();}



	// Update zombies

	for(int z=0; z<(int)zombieList.size(); z++)
	{
		if(!zombieList[z].dead)
		{
			// Unmark old cell
			if(zombieList[z].cellZombie != NULL)
			{
				zombieList[z].cellZombie->isZombie = false;
				zombieList[z].cellZombie->ptrZombie = NULL;
			}

			// Set currnet cell
			int tilex, tiley;
			tilex = ut::round(zombieList[z].x/16);
			tiley = ut::round(zombieList[z].y/16);
			zombieList[z].cellZombie = &cell_data[getCellIndex(tilex,tiley)];
			zombieList[z].cellZombie->isZombie = true;
			zombieList[z].cellZombie->ptrZombie = &zombieList[z];

			// Individual logic
			zombieList[z].step();
		}
		else if(zombieList[z].dead == true && zombieList[z].proc == false)
		{
			zombieList[z].proc = true;
			zombieCount --;

			zombieList[z].cellZombie->isZombie = false;
			zombieList[z].cellZombie->ptrZombie = NULL;
		}
	}
	

}








///
// Re-lights the level by updateing the drawList
// No loops because I had no idea what I was doing when I wrote this...
///
void level::doLight()
{

	if(!lights)
	{

		drawList.clear();

		for(int ypos=0; ypos<C_MAP_HEIGHT_IN_TILES; ypos++)
		{
			for(int xpos=0; xpos<C_MAP_WIDTH_IN_TILES; xpos++)
			{
					drawList.push_back(&cell_data[getCellIndex(xpos,ypos)]);
			}
		}

		return;

	}

	// clear the draw list
	if(count >= 2)
	{
		drawList.clear();
		drawList = drawBuf;
		drawBuf.clear();
		count = 0;
	}

	// Update lantern hack
	int nmlt = lanternList.size();
	if(nmlt != 0)
	{
		hf_1 = (int)(nmlt/2);
		hf_2 = nmlt-hf_1;

		hf_1 = 0;
		hf_2 --;
	}
	else
	{
		hf_1 = 0;
		hf_2 = 0;
	}


	// Mark all cells as unlit
	for(int t=0; t<C_MAP_TOTAL_SIZE; t++)
	{
		cell_data[t].lit = false;
	}

	
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












	////////
	////////	Lanterns
	////////


	// Hack
	int start, limit;
	if(!whichHalf)
	{
		start = hf_1;
		limit = hf_2;
	}
	else
	{
		start = hf_2;
		limit = lanternList.size();
	}
	// </hack>

	for(int t=start; t<limit; t++)
	{
		// for every lantern in the level

		// Hack crash check
		if(lanternList.size() == 0)
			break;

		// handle straight lines
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y);


		// handle lines in an up+left direction
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-1, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-2, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-3, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-4, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-5, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-6, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-7, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-8, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);

		// handle lines in an up+right direction
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+1, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+2, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+3, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+4, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+5, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+6, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+7, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+8, lanternList[t]->y-C_LANTERN_LIGHT_DISTANCE);




		// handle lines in an down+left direction
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-1, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-2, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-3, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-4, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-5, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-6, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-7, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-8, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);

		// handle lines in an down+right direction
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+1, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+2, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+3, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+4, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+5, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+6, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+7, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+8, lanternList[t]->y+C_LANTERN_LIGHT_DISTANCE);




		// handle lines in an left+down direction
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+1);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+2);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+3);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+4);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+5);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+6);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+7);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+8);

		// handle lines in an left+up direction
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-1);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-2);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-3);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-4);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-5);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-6);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-7);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x-C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-8);




		// handle lines in an right+down direction
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+1);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+2);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+3);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+4);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+5);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+6);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+7);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y+8);

		// handle lines in an right+up direction
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-1);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-2);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-3);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-4);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-5);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-6);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-7);
		handleLineB(lanternList[t]->x, lanternList[t]->y, lanternList[t]->x+C_LANTERN_LIGHT_DISTANCE, lanternList[t]->y-8);
	}


	// Hack
	whichHalf = !whichHalf;
	count ++;


}















void level::handleLine(int xx0, int yy0, int xx1, int yy1)
{
	std::vector<sf::Vector2i> tmp = ut::calculateLine(xx0,yy0,xx1,yy1);
	for(int t=0; t<tmp.size(); t++)
	{
		// For each cell in the line
		// Mark it as lit
		// Add it to the draw list and stop if it's a block

		//cell_data[getCellIndex(tmp[t].x,tmp[t].y)].lit = true;
		drawList.push_back(&cell_data[getCellIndex(tmp[t].x,tmp[t].y)]);

		if(cell_data[getCellIndex(tmp[t].x,tmp[t].y)].type == 1 || cell_data[getCellIndex(tmp[t].x,tmp[t].y)].type == 2)
			break;
	}
}


void level::handleLineB(int xx0, int yy0, int xx1, int yy1)
{
	std::vector<sf::Vector2i> tmp = ut::calculateLine(xx0,yy0,xx1,yy1);
	for(int t=0; t<tmp.size(); t++)
	{
		// For each cell in the line
		// Mark it as lit
		// Add it to the draw list and stop if it's a block

		//cell_data[getCellIndex(tmp[t].x,tmp[t].y)].lit = true;
		drawBuf.push_back(&cell_data[getCellIndex(tmp[t].x,tmp[t].y)]);

		if(cell_data[getCellIndex(tmp[t].x,tmp[t].y)].type == 1 || cell_data[getCellIndex(tmp[t].x,tmp[t].y)].type == 2)
			break;
	}
}