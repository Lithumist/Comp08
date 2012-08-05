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
}

void player::init(){init(0,0);}



///
// Handles the events
///
void player::events(sf::Event* evnt)
{
	// update movement varibles
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		keyUp = true; else keyUp = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		keyDown = true; else keyDown = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		keyLeft = true; else keyLeft = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		keyRight = true; else keyRight = false;
}



///
// Does logic
///
void player::step()
{
	// update speeds
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

	// move player
	x += xspeed;
	y += yspeed;
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

	global::rwpWindow->draw(rct);
}