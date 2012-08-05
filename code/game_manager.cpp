/*
	game_manager.h

	Defines a class that manages the gameplay.
*/

#include "game_manager.h"



///
// Terminates the current game and starts a new one
///
void gameManager::newGame()
{
	// Eeset the player and set the global pointer
	gamePlayer.init(0,0);
	global::plPlayer = &gamePlayer;
}



///
// Handles events
///
void gameManager::events(sf::Event* evnt)
{
	// Do player events
	global::plPlayer->events(evnt);
}



///
// Does logic
///
void gameManager::step()
{
	// Do player logic
	global::plPlayer->step();
}



///
// Renders the game
///
void gameManager::draw()
{
	// Do player events
	global::plPlayer->draw();
}