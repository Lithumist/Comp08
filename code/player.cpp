/*
	player.h

	Defines a namespace containing useful utility functions that
	the rest of the game uses.
*/

#include "player.h"


float PLAYER_MINE_TIME = 0.8;


///
// Reset's the player
///
void player::init(float xpos, float ypos)
{
	x = xpos;
	y = ypos;
	xspeed = 0;
	yspeed = 0;

	hasSword = false;
	
	keyUp = false;
	keyDown = false;
	keyLeft = false;
	keyRight = false;

	rmCell = false;
	placeCell = false;
	rmLantern = false;
	placeLantern = false;
	centre = false;

	fastMine = false;

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
	numberLanterns = 2;
	global::intPlayerLanterns = &numberLanterns;
	numberTreasure = 0;
	global::intPlayerTreasure = &numberTreasure;

	global::flPlayerX = &x;
	global::flPlayerY = &y;

	liveRectPlayer.width = 16;
	liveRectPlayer.height = 16;

	hasReset = false;

	hp = 100;

	attack = false;

	SND_mine.setBuffer(global::SNDBUF_mine); SND_mine.setLoop(false);
	SND_hit.setBuffer(global::SNDBUF_player_hit); SND_hit.setLoop(false);
	SND_miss.setBuffer(global::SNDBUF_player_miss); SND_miss.setLoop(false);
	SND_hurt.setBuffer(global::SNDBUF_player_hurt); SND_hurt.setLoop(false);
	SND_sword.setBuffer(global::SNDBUF_get_sword); SND_sword.setLoop(false);

	SPR_up.setTexture(global::TXT_player_up);
	SPR_down.setTexture(global::TXT_player_down);
	SPR_left.setTexture(global::TXT_player_left);
	SPR_right.setTexture(global::TXT_player_right);
	SPR_down_sword.setTexture(global::TXT_player_down_sword);
	SPR_right_sword.setTexture(global::TXT_player_right_sword);
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


	// Update attack varible
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		attack = true;
	else
		attack = false;


	// Update centre varible
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		centre = true;
	else
		centre = false;




	// Update fast mine
	/*
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) && !fastMine)
		fastMine = true;
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num6) && fastMine)
		fastMine = false;
		*/
}



///
// Does logic
///
void player::step()
{
	// Update fast mine
	/*
	if(fastMine)
	{
		fastMine = false;
		if(PLAYER_MINE_TIME == 0.8)
			PLAYER_MINE_TIME = 0.1;
		else
			PLAYER_MINE_TIME = 0.8;
	}
	*/


	// Give sword
	if(numberTreasure >= 100 && !hasSword)
	{
		hasSword = true;
		SND_sword.play();
		std::cout << "You have obtained a sword!\n";
	}


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

	// Get hit by zombies if needed
	if(global::zombieHitPlayerFlag)
	{
		//numberTreasure += 100;///////////////////////////
		SND_hurt.play();

		global::zombieHitPlayerFlag = false;
		hurt(global::zombieDmg);
		float revX, revY;
		revX = global::zombieX*4;
		revY = global::zombieY*4;

		xspeed += revX;
		yspeed += revY;
	}

	// Move player
	x += xspeed * FpsCalc::GetInstance()->speedfactor;
	y += yspeed * FpsCalc::GetInstance()->speedfactor;


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


	// centre player if needed
	if(centre)
	{
		centre = false;
		x = cellPlayer.left;
		y = cellPlayer.top;
	}


	// Handle attacking
	if(attack && attackTime.getElapsedTime().asSeconds() > C_ATTACK_TIME_DELAY)
	{

		switch(dir)
		{
			case 1:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].isZombie)
				{
					// hit
					zombie* zptr = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].ptrZombie;

					sf::Vector2f vtz;
					vtz.x = zptr->x - x;
					vtz.y = zptr->y - y;

					SND_hit.play();

					int finalDmg = C_PLAYER_DAMAGE;
					if(hasSword)
						finalDmg += C_PLAYER_SWORD_DAMAGE_BOOST;

					zptr->damage(vtz,finalDmg);
				}
				else
				{
					// miss
					//std::cout << "miss\n";
					SND_miss.play();
				}
			break;

			case 2:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].isZombie)
				{
					// hit
					zombie* zptr = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].ptrZombie;

					sf::Vector2f vtz;
					vtz.x = zptr->x - x;
					vtz.y = zptr->y - y;

					SND_hit.play();

					zptr->damage(vtz,C_PLAYER_DAMAGE);
				}
				else
				{
					// miss
					//std::cout << "miss\n";
					SND_miss.play();
				}
			break;

			case 3:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].isZombie)
				{
					// hit
					zombie* zptr = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].ptrZombie;

					sf::Vector2f vtz;
					vtz.x = zptr->x - x;
					vtz.y = zptr->y - y;

					SND_hit.play();

					zptr->damage(vtz,C_PLAYER_DAMAGE);
				}
				else
				{
					// miss
					//std::cout << "miss\n";
					SND_miss.play();
				}
			break;

			case 4:
				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].isZombie)
				{
					// hit
					zombie* zptr = global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].ptrZombie;

					sf::Vector2f vtz;
					vtz.x = zptr->x - x;
					vtz.y = zptr->y - y;

					SND_hit.play();

					zptr->damage(vtz,C_PLAYER_DAMAGE);
				}
				else
				{
					// miss
					//std::cout << "miss\n";
					SND_miss.play();
				}
			break;
		}

		attack = false;
		attackTime.restart();
	}


	// Handle lantern removal
	if(rmLantern)
	{
		rmLantern = false;
		switch(dir)
		{
			case 1:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type == 3)
				{
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type = 0;
					numberLanterns ++;
					for(int t=0; t<global::lvlLevel->lanternList.size(); t++)
					{
						if(global::lvlLevel->lanternList[t]->x == global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].x && global::lvlLevel->lanternList[t]->y == global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].y)
						{
							global::lvlLevel->lanternList.erase(global::lvlLevel->lanternList.begin()+t);
							break;
						}
					}
				}

			break;

			case 2:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type == 3)
				{
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type = 0;
					numberLanterns ++;
					for(int t=0; t<global::lvlLevel->lanternList.size(); t++)
					{
						if(global::lvlLevel->lanternList[t]->x == global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].x && global::lvlLevel->lanternList[t]->y == global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].y)
						{
							global::lvlLevel->lanternList.erase(global::lvlLevel->lanternList.begin()+t);
							break;
						}
					}
				}

			break;

			case 3:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type == 3)
				{
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type = 0;
					numberLanterns ++;
					for(int t=0; t<global::lvlLevel->lanternList.size(); t++)
					{
						if(global::lvlLevel->lanternList[t]->x == global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].x && global::lvlLevel->lanternList[t]->y == global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].y)
						{
							global::lvlLevel->lanternList.erase(global::lvlLevel->lanternList.begin()+t);
							break;
						}
					}
				}

			break;

			case 4:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type == 3)
				{
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type = 0;
					numberLanterns ++;
					for(int t=0; t<global::lvlLevel->lanternList.size(); t++)
					{
						if(global::lvlLevel->lanternList[t]->x == global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].x && global::lvlLevel->lanternList[t]->y == global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].y)
						{
							global::lvlLevel->lanternList.erase(global::lvlLevel->lanternList.begin()+t);
							break;
						}
					}
				}

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

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type == 0 && numberLanterns > 0)
				{
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)].type = 3;
					numberLanterns --;
					global::lvlLevel->lanternList.push_back(&global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)]);
				}

			break;

			case 2:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type == 0 && numberLanterns > 0)
				{
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)].type = 3;
					numberLanterns --;
					global::lvlLevel->lanternList.push_back(&global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)]);
				}

			break;

			case 3:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type == 0 && numberLanterns > 0)
				{
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)].type = 3;
					numberLanterns --;
					global::lvlLevel->lanternList.push_back(&global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)]);
				}

			break;

			case 4:

				if(global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type == 0 && numberLanterns > 0)
				{
					global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)].type = 3;
					numberLanterns --;
					global::lvlLevel->lanternList.push_back(&global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)]);
				}

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
	if(hasReset && cellTime.getElapsedTime().asSeconds() >= PLAYER_MINE_TIME && rmCell)
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


	// Update sf::Listener position and all other player-handled sounds

	sf::Listener::setPosition(x,y,0);
	SND_mine.setPosition(x,y,0);
	SND_hit.setPosition(x,y,0);
	SND_miss.setPosition(x,y,0);
	SND_hurt.setPosition(x,y,0);
	SND_sword.setPosition(x,y,0);




}




///
// Render's the player
///
void player::draw()
{

	// Draw player

	switch(dir)
	{
		case 1: // up
			SPR_up.setPosition(x,y);
			global::rwpWindow->draw(SPR_up);
		break;

		case 2: // right
			if(hasSword)
			{
				SPR_right_sword.setPosition(x,y);
				global::rwpWindow->draw(SPR_right_sword);
			}
			else
			{
				SPR_right.setPosition(x,y);
				global::rwpWindow->draw(SPR_right);
			}
		break;

		case 3: // down
			if(hasSword)
			{
				SPR_down_sword.setPosition(x,y);
				global::rwpWindow->draw(SPR_down_sword);
			}
			else
			{
				SPR_down.setPosition(x,y);
				global::rwpWindow->draw(SPR_down);
			}
		break;

		case 4: // left
			SPR_left.setPosition(x,y);
			global::rwpWindow->draw(SPR_left);
		break;
	}



	// draw the player as a green square
	/*
	sf::RectangleShape rct;
	rct.setPosition(x,y);
	rct.setSize(sf::Vector2f(16,16));
	rct.setFillColor(sf::Color(0,255,0));

	global::rwpWindow->draw(rct);
	*/


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

	/*
	// Draw block number text
	sf::Text blktxt("blocks: " + ut::toString(numberBlocks));
	blktxt.setFont(global::fntMain);
	blktxt.setCharacterSize(12);
	global::rwpWindow->draw(blktxt);
	*/


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
	bool doChest = false;
	mapCell* ptrChest;

	mapCell* ptrUp;
	mapCell* ptrDown;
	mapCell* ptrLeft;
	mapCell* ptrRight;

	ptrUp = &global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellUp.left/16,cellUp.top/16)];
	ptrDown = &global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellDown.left/16,cellDown.top/16)];
	ptrLeft = &global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellLeft.left/16,cellLeft.top/16)];
	ptrRight = &global::lvlLevel->cell_data[global::lvlLevel->getCellIndex(cellRight.left/16,cellRight.top/16)];

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

	if(upType == 1 || upType == 2 || upType == 4 || upType == 3)
	{
		if(liveRectPlayer.intersects(cellUp))
		{
			// y = cellPlayer.top;
			// x = cellPlayer.left;
			x -= xspeed * FpsCalc::GetInstance()->speedfactor; xspeed = 0;
			//y -= yspeed; yspeed = 0;
			y = cellPlayer.top; yspeed = 0;

			if(upType == 4)
			{
				doChest = true;
				ptrChest = ptrUp;
			}

		}
	}

	if(downType == 1 || downType == 2 || downType == 4 || downType == 3)
	{
		if(liveRectPlayer.intersects(cellDown))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			x -= xspeed * FpsCalc::GetInstance()->speedfactor; xspeed = 0;
			//y -= yspeed; yspeed = 0;
			y = cellPlayer.top; yspeed = 0;

			if(downType == 4)
			{
				doChest = true;
				ptrChest = ptrDown;
			}

		}
	}

	if(leftType == 1 || leftType == 2 || leftType == 4 || leftType == 3)
	{
		if(liveRectPlayer.intersects(cellLeft))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			//x -= xspeed; xspeed = 0;
			y -= yspeed * FpsCalc::GetInstance()->speedfactor; yspeed = 0;
			x = cellPlayer.left; xspeed = 0;

			if(leftType == 4)
			{
				doChest = true;
				ptrChest = ptrLeft;
			}

		}
	}

	if(rightType == 1 || rightType == 2 || rightType == 4 || rightType == 3)
	{
		if(liveRectPlayer.intersects(cellRight))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			//x -= xspeed; xspeed = 0;
			y -= yspeed * FpsCalc::GetInstance()->speedfactor; yspeed = 0;
			x = cellPlayer.left; xspeed = 0;

			if(rightType == 4)
			{
				doChest = true;
				ptrChest = ptrRight;
			}

		}
	}

	if(topLeftType == 1 || topLeftType == 2)
	{
		if(liveRectPlayer.intersects(cellTopLeft))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			x -= xspeed * FpsCalc::GetInstance()->speedfactor; xspeed = 0;
			y -= yspeed * FpsCalc::GetInstance()->speedfactor; yspeed = 0;
		}
	}

	if(topRightType == 1 || topRightType == 2)
	{
		if(liveRectPlayer.intersects(cellTopRight))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			x -= xspeed * FpsCalc::GetInstance()->speedfactor; xspeed = 0;
			y -= yspeed * FpsCalc::GetInstance()->speedfactor; yspeed = 0;
		}
	}

	if(bottomLeftType == 1 || bottomLeftType == 2)
	{
		if(liveRectPlayer.intersects(cellBottomLeft))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			x -= xspeed * FpsCalc::GetInstance()->speedfactor; xspeed = 0;
			y -= yspeed * FpsCalc::GetInstance()->speedfactor; yspeed = 0;
		}
	}

	if(bottomRightType == 1 || bottomRightType == 2)
	{
		if(liveRectPlayer.intersects(cellBottomRight))
		{
			//y = cellPlayer.top;
			//x = cellPlayer.left;
			x -= xspeed * FpsCalc::GetInstance()->speedfactor; xspeed = 0;
			y -= yspeed * FpsCalc::GetInstance()->speedfactor; yspeed = 0;
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




	// Handle collision to chests
	if(doChest)
	{

		doChest = false;
		if(!ptrChest->looted)
		{
			ptrChest->looted = true;
			numberTreasure += ptrChest->treasure;
			numberLanterns += ptrChest->lanterns;
		}

	}



}