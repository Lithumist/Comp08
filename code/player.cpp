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

	dir = 3; // down

	numberBlocks = 0;
	global::intPlayerBlocks = &numberBlocks;
}

void player::init(){init(0,0);}



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
	if(keyDown)
		yspeed += C_PLAYER_SPEED;
	if(keyLeft)
		xspeed -= C_PLAYER_SPEED;
	if(keyRight)
		xspeed += C_PLAYER_SPEED;

	// Move player
	x += xspeed;
	y += yspeed;


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


	// Remove cell if needed
	if(rmCell)
	{
		switch(dir)
		{
			case 1:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].active != false)
				{
					numberBlocks ++;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].active = false;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type = 0;
					//recalculate light here
				}
			break;

			case 2:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].active != false)
				{
					numberBlocks ++;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].active = false;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type = 0;
					//recalculate light here
				}
			break;

			case 3:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].active != false)
				{
					numberBlocks ++;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].active = false;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type = 0;
					//recalculate light here
				}
			break;

			case 4:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].active != false)
				{
					numberBlocks ++;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].active = false;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type = 0;
					//recalculate light here
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
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].active == false && numberBlocks > 0)
				{
					numberBlocks --;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].active = true;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type = 1;
					//recalculate light here
				}
			break;

			case 2:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].active == false && numberBlocks > 0)
				{
					numberBlocks --;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].active = true;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type = 1;
					//recalculate light here
				}
			break;

			case 3:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].active == false && numberBlocks > 0)
				{
					numberBlocks --;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].active = true;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type = 1;
					//recalculate light here
				}
			break;

			case 4:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].active == false && numberBlocks > 0)
				{
					numberBlocks --;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].active = true;
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type = 1;
					//recalculate light here
				}
			break;
		}
	}
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


	// Draw block number text
	sf::Text blktxt("blocks: " + ut::toString(numberBlocks));
	blktxt.setFont(global::fntMain);
	blktxt.setCharacterSize(12);
	global::rwpWindow->draw(blktxt);
}