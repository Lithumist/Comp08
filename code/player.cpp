/*
	player.h

	Defines a namespace containing useful utility functions that
	the rest of the game uses.
*/

#include "player.h"





///
// Reset's the player
///
void player::init(float xpos, float ypos)
{
	x = xpos;
	y = ypos;
	xspeed = 0;
	yspeed = 0;
	
	keyUp = false;
	keyDown = false;
	keyLeft = false;
	keyRight = false;

	rmCell = false;
	placeCell = false;
	rmLantern = false;
	placeLantern = false;

	cellPlayer.width = 16;
	cellPlayer.height = 16;
	cellLeft.width = 16;
	cellLeft.height = 16;
	cellRight.width = 16;
	cellRight.height = 16;
	cellUp.width = 16;
	cellUp.height = 16;
	cellDown.width = 16;
	cellDown.height = 16;
	cellTopLeft.width = 16;
	cellTopLeft.height = 16;
	cellTopRight.width = 16;
	cellTopRight.height = 16;
	cellBottomLeft.width = 16;
	cellBottomLeft.height = 16;
	cellBottomRight.width = 16;
	cellBottomRight.height = 16;

	dir = 3; // down

	numberBlocks = 0;
	global::intPlayerBlocks = &numberBlocks;
	numberLanterns = 5;
	global::intPlayerLanterns = &numberLanterns;
	numberTreasure = 0;
	global::intPlayerTreasure = &numberTreasure;

	global::flPlayerX = &x;
	global::flPlayerY = &y;

	liveRectPlayer.width = 16;
	liveRectPlayer.height = 16;

	hasReset = false;

	hp = 100;

	SND_mine.setBuffer(global::SNDBUF_mine); SND_mine.setLoop(false);
}

void player::init(){init(0,0);}

void player::initNextLevel(float xpos, float ypos)
{
	x = xpos;
	y = ypos;
	xspeed = 0;
	yspeed = 0;
	
	keyUp = false;
	keyDown = false;
	keyLeft = false;
	keyRight = false;

	rmCell = false;
	placeCell = false;
	rmLantern = false;
	placeLantern = false;

	dir = 3; // down

	hasReset = false;
}



///
// Handles the events
///
void player::events(sf::Event* evnt)
{
	// update movement varibles
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{keyUp = true; dir = 1;} else keyUp = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{keyDown = true; dir = 3;} else keyDown = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{keyLeft = true; dir = 4;} else keyLeft = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{keyRight = true; dir = 2;} else keyRight = false;

	// Update cell removal varible
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		rmCell = true;
	else
		rmCell = false;

	// Update cell placing varibles
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		placeCell = true;
	else
		placeCell = false;


	// Update lantern removal varible
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		rmLantern = true;
	else
		rmLantern = false;

	// Update lantern placing varibles
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		placeLantern = true;
	else
		placeLantern = false;
}



///
// Does logic
///
void player::step()
{
	// Update speeds
	xspeed = 0;
	yspeed = 0;
	if(keyUp)
		yspeed -= C_PLAYER_SPEED;
	else if(keyDown)
		yspeed += C_PLAYER_SPEED;
	else if(keyLeft)
		xspeed -= C_PLAYER_SPEED;
	else if(keyRight)
		xspeed += C_PLAYER_SPEED;

	// Move player
	x += xspeed;
	y += yspeed;


	// Update live player rect
	liveRectPlayer.left = x;
	liveRectPlayer.top = y;


	// Update rects
	cellPlayer.left = ut::round(x/16)*16;
	cellPlayer.top = ut::round(y/16)*16;

	cellLeft.left = cellPlayer.left-16;
	cellLeft.top = cellPlayer.top;

	cellRight.left = cellPlayer.left+16;
	cellRight.top = cellPlayer.top;

	cellUp.left = cellPlayer.left;
	cellUp.top = cellPlayer.top-16;

	cellDown.left = cellPlayer.left;
	cellDown.top = cellPlayer.top+16;

	cellTopLeft.left = cellPlayer.left-16;
	cellTopLeft.top = cellPlayer.top-16;

	cellTopRight.left = cellPlayer.left+16;
	cellTopRight.top = cellPlayer.top-16;

	cellBottomLeft.left = cellPlayer.left-16;
	cellBottomLeft.top = cellPlayer.top+16;

	cellBottomRight.left = cellPlayer.left+16;
	cellBottomRight.top = cellPlayer.top+16;


	// Handle lantern removal
	if(rmLantern)
	{
		rmLantern = false;
		switch(dir)
		{
			case 1:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type == 3)
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type = 0;

			break;

			case 2:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type == 3)
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type = 0;

			break;

			case 3:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type == 3)
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type = 0;

			break;

			case 4:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type == 3)
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type = 0;

			break;
		}
	}



	// Handle lantern placing
	if(placeLantern)
	{
		placeLantern = false;
		switch(dir)
		{
			case 1:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type == 0)
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type = 3;

			break;

			case 2:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type == 0)
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type = 3;

			break;

			case 3:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type == 0)
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type = 3;

			break;

			case 4:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type == 0)
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type = 3;

			break;
		}
	}









	// Update cellTime and stuff to do with it
	if(rmCell && !hasReset)
	{
		switch(dir)
		{
			case 1:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type == 1)
				{
					hasReset = true;
					cellTime.restart();
					SND_mine.play();
				}
			break;

			case 2:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type == 1)
				{
					hasReset = true;
					cellTime.restart();
					SND_mine.play();
				}
			break;

			case 3:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type == 1)
				{
					hasReset = true;
					cellTime.restart();
					SND_mine.play();
				}
			break;

			case 4:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type == 1)
				{
					hasReset = true;
					cellTime.restart();
					SND_mine.play();
				}
			break;
		}
	}

	// Abort?
	if(!rmCell && hasReset)
	{
		hasReset = false;
		SND_mine.stop();
	}


	// Remove cell if needed
	// OPTIMISE could possible remove the large switch statment because the previous code allready does it and checks
	if(hasReset && cellTime.getElapsedTime().asSeconds() >= 0.8 && rmCell)
	{
		hasReset = false;
		switch(dir)
		{
			case 1:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type == 1)
				{
					numberBlocks ++;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type = 0;
				}
			break;

			case 2:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type == 1)
				{
					numberBlocks ++;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type = 0;
				}
			break;

			case 3:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type == 1)
				{
					numberBlocks ++;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type = 0;
				}
			break;

			case 4:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type == 1)
				{
					numberBlocks ++;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type = 0;
				}
			break;
		}
	}





	// Place cell if needed
	if(placeCell)
	{
		switch(dir)
		{
			case 1:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type == 0 && numberBlocks > 0)
				{
					numberBlocks --;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type = 1;
					//recalculate light here
				}
			break;

			case 2:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type == 0&& numberBlocks > 0)
				{
					numberBlocks --;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type = 1;
					//recalculate light here
				}
			break;

			case 3:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type == 0&& numberBlocks > 0)
				{
					numberBlocks --;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type = 1;
					//recalculate light here
				}
			break;

			case 4:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type == 0&& numberBlocks > 0)
				{
					numberBlocks --;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type = 1;
					//recalculate light here
				}
			break;
		}
	}







	// Detect and handle collisions
	handleCollisions();




}




///
// Render's the player
///
void player::draw()
{
	// draw the player as a green square
	sf::RectangleShape rct;
	rct.setPosition(x,y);
	rct.setSize(sf::Vector2f(16,16));
	rct.setFillColor(sf::Color(0,255,0));

	global::rwpWindow->draw(rct);


	// Draw rects
	/*
	sf::RectangleShape rct1;
	rct1.setPosition(cellPlayer.left,cellPlayer.top);
	rct1.setSize(sf::Vector2f(16,16));
	rct1.setOutlineColor(sf::Color(255,255,255));
	rct1.setOutlineThickness(1);
	rct1.setFillColor(sf::Color::Transparent);
	global::rwpWindow->draw(rct1);


	sf::RectangleShape rct2;
	rct2.setPosition(cellLeft.left,cellLeft.top);
	rct2.setSize(sf::Vector2f(16,16));
	if(dir==4)
		rct2.setOutlineColor(sf::Color(0,0,255));
	else
		rct2.setOutlineColor(sf::Color(128,128,128));
	rct2.setOutlineThickness(1);
	rct2.setFillColor(sf::Color::Transparent);
	global::rwpWindow->draw(rct2);

	rct2.setPosition(cellRight.left,cellRight.top);
	rct2.setSize(sf::Vector2f(16,16));
	if(dir==2)
		rct2.setOutlineColor(sf::Color(0,0,255));
	else
		rct2.setOutlineColor(sf::Color(128,128,128));
	rct2.setOutlineThickness(1);
	rct2.setFillColor(sf::Color::Transparent);
	global::rwpWindow->draw(rct2);

	rct2.setPosition(cellUp.left,cellUp.top);
	rct2.setSize(sf::Vector2f(16,16));
	if(dir==1)
		rct2.setOutlineColor(sf::Color(0,0,255));
	else
		rct2.setOutlineColor(sf::Color(128,128,128));
	rct2.setOutlineThickness(1);
	rct2.setFillColor(sf::Color::Transparent);
	global::rwpWindow->draw(rct2);

	rct2.setPosition(cellDown.left,cellDown.top);
	rct2.setSize(sf::Vector2f(16,16));
	if(dir==3)
		rct2.setOutlineColor(sf::Color(0,0,255));
	else
		rct2.setOutlineColor(sf::Color(128,128,128));
	rct2.setOutlineThickness(1);
	rct2.setFillColor(sf::Color::Transparent);
	global::rwpWindow->draw(rct2);

	*/


	// Draw block number text
	sf::Text blktxt("blocks: " + ut::toString(numberBlocks));
	blktxt.setFont(global::fntMain);
	blktxt.setCharacterSize(12);
	global::rwpWindow->draw(blktxt);


	// Draw the collision points
	/*
	for(int t=0; t<collisionTestList.size(); t++)
	{
		sf::RectangleShape tmp;
		tmp.setPosition(collisionTestList[t].x, collisionTestList[t].y);
		tmp.setFillColor(sf::Color(255,255,255));
		tmp.setSize(sf::Vector2f(1,1));

		global::rwpWindow->draw(tmp);
	}
	*/
}


///
// Kills the player instantly
///
void player::kill()
{
	hp = 0;
	global::gsGameState = S_DEAD;
}


// Hurts the player
void player::hurt(int damage)
{
	hp -= damage;

	if(hp <= 0)
		kill();
}

// Heals the player
void player::heal(int ammount)
{
	hp += ammount;
}

// Returns the current HP
int player::getHp(){return hp;}

























void player::handleCollisions()
{

	// Handle collisions to walls

	int upType, downType, leftType, rightType, topLeftType, topRightType, bottomLeftType, bottomRightType;

	upType = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type;
	downType = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type;
	leftType = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type;
	rightType = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type;
	topLeftType = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellTopLeft.left/16,cellTopLeft.top/16)].type;
	topRightType = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellTopRight.left/16,cellTopRight.top/16)].type;
	bottomLeftType = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellBottomLeft.left/16,cellBottomLeft.top/16)].type;
	bottomRightType = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellBottomRight.left/16,cellBottomRight.top/16)].type;

	if(upType == 1 || upType == 2)
	{
		if(liveRectPlayer.intersects(cellUp))
		{
			// y = cellPlayer.top;
			// x = cellPlayer.left;
			x -= xspeed; xspeed = 0;
			//y -= yspeed; yspeed = 0;
			y = cellPlayer.top; yspeed = 0;
		}
	}

	if(downType == 1 || downType == 2)
	{
		if(liveRectPlayer.intersects(cellDown))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			x -= xspeed; xspeed = 0;
			//y -= yspeed; yspeed = 0;
			y = cellPlayer.top; yspeed = 0;
		}
	}

	if(leftType == 1 || leftType == 2)
	{
		if(liveRectPlayer.intersects(cellLeft))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			//x -= xspeed; xspeed = 0;
			y -= yspeed; yspeed = 0;
			x = cellPlayer.left; xspeed = 0;
		}
	}

	if(rightType == 1 || rightType == 2)
	{
		if(liveRectPlayer.intersects(cellRight))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			//x -= xspeed; xspeed = 0;
			y -= yspeed; yspeed = 0;
			x = cellPlayer.left; xspeed = 0;
		}
	}

	if(topLeftType == 1 || topLeftType == 2)
	{
		if(liveRectPlayer.intersects(cellTopLeft))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			x -= xspeed; xspeed = 0;
			y -= yspeed; yspeed = 0;
		}
	}

	if(topRightType == 1 || topRightType == 2)
	{
		if(liveRectPlayer.intersects(cellTopRight))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			x -= xspeed; xspeed = 0;
			y -= yspeed; yspeed = 0;
		}
	}

	if(bottomLeftType == 1 || bottomLeftType == 2)
	{
		if(liveRectPlayer.intersects(cellBottomLeft))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			x -= xspeed; xspeed = 0;
			y -= yspeed; yspeed = 0;
		}
	}

	if(bottomRightType == 1 || bottomRightType == 2)
	{
		if(liveRectPlayer.intersects(cellBottomRight))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			x -= xspeed; xspeed = 0;
			y -= yspeed; yspeed = 0;
		}
	}




	// Update live player rect
	liveRectPlayer.left = x;
	liveRectPlayer.top = y;




	// Handle collision to exitCell
	sf::IntRect exitRect;
	exitRect.left = global::flExitCellX;
	exitRect.top = global::flExitCellY;
	exitRect.width = 16;
	exitRect.height = 16;

	if(liveRectPlayer.intersects(exitRect))
	{
		// The level must now end
		global::blEndLevelTrigger = true;
	}



}