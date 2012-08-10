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
	// Generate a new level and set global pointer
	gameLevel.generate();
	global::lvlLevel = &gameLevel;

	// Reset the player and set the global pointer
	gamePlayer.init(gameLevel.getPlayerStartX()*16,gameLevel.getPlayerStartY()*16);
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
	// Do level logic
	global::lvlLevel->step();

	// Do player logic
	global::plPlayer->step();
}



///
// Renders the game
///
void gameManager::draw()
{
	// Render the map
	global::lvlLevel->draw();

	// Render the player
	global::plPlayer->draw();
}