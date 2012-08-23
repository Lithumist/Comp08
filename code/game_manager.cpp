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
	// Set some varibles
	currentLevel = 1;
	global::ptrCurLevel = &currentLevel;
	doPause = false;
	toggleLight = false;

	// Init
	gameLevel.init();

	// Generate a new level and set global pointer
	gameLevel.generate();
	global::lvlLevel = &gameLevel;

	// Calculate lights = true
	global::lvlLevel->lights = true;

	// Reset the player and set the global pointer
	gamePlayer.init(gameLevel.getPlayerStartX()*16,gameLevel.getPlayerStartY()*16);
	global::plPlayer = &gamePlayer;
}


///
// Resets the game, but going to the next level
///
void gameManager::nextLevel()
{
	// Increment the level counter
	currentLevel ++;

	// Set some varibles
	doPause = false;

	// Generate a new level
	gameLevel.generate();

	// Reset the player
	gamePlayer.initNextLevel(gameLevel.getPlayerStartX()*16,gameLevel.getPlayerStartY()*16);
}



///
// Handles events
///
void gameManager::events(sf::Event* evnt)
{
	// Update pause trigger
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		doPause = true;
	else
		doPause = false;


	// Debug
	/*
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
		gamePlayer.hurt(10);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
		gamePlayer.heal(10);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
		gamePlayer.kill();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
		nextLevel();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
		toggleLight = true;
	else
		toggleLight = false;

	*/


	// Do player events
	global::plPlayer->events(evnt);
}



///
// Does logic
///
void gameManager::step()
{
	// Do light toggling logic
	if(toggleLight)
	{
		toggleLight = false;
		global::lvlLevel->lights = !global::lvlLevel->lights;
	}



	// Do pause logic
	if(doPause)
	{
		doPause = false;
		global::bGamePause = true;
		global::gsGameState = S_PAUSE;
	}
	else
	{

	// Check for end level
	if(global::blEndLevelTrigger)
	{
		global::blEndLevelTrigger = false;
		nextLevel();
	}

	// Do level logic
	global::lvlLevel->step();

	// Do player logic
	global::plPlayer->step();

	}
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