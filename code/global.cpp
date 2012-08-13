/*
	global.cpp

	Defines a namespace of global varibles and functions
	that the rest of the game depend upon.
*/

#include "global.h"

namespace global
{

	// A pointer to the RenderWindow the game uses and draws to.
	sf::RenderWindow* rwpWindow = NULL;

	// If this is true, the main loop is running
	bool bLoopRunning = true;

	// The state of the game
	GAME_STATE gsGameState = S_MENU;

	// A pointer to the current player object that is contained inside the gameManager
	player* plPlayer = NULL;
	// Pointers to the player positions
	float* flPlayerX = NULL;
	float* flPlayerY = NULL;

	// A pointer to the current level object that is contained inside the gameManager
	level* lvlLevel = NULL;

	// A pointer to the number of blocks the player has
	int* intPlayerBlocks = NULL;

	// The main font used in the game
	sf::Font fntMain;

	// The pause game trigger and indicator
	bool bGamePause = false;

	// The position of the exit cell (pixel coordinates)
	float flExitCellX;
	float flExitCellY;

	// The trigger to end the level and go to the next one
	bool blEndLevelTrigger = false;


	// All sound data used
	sf::SoundBuffer SNDBUF_mine;


} // End namespace