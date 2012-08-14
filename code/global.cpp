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

	// Pointers to the number of blocks, lanterns and treasure the player has
	int* intPlayerBlocks = NULL;
	int* intPlayerLanterns = NULL;
	int* intPlayerTreasure = NULL;

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



	// All texture data used
	sf::Texture TXT_player_down;
	sf::Texture TXT_player_up;
	sf::Texture TXT_player_left;
	sf::Texture TXT_player_right;

	sf::Texture TXT_wall;
	sf::Texture TXT_u_wall;
	sf::Texture TXT_exit;
	sf::Texture TXT_lantern;
	sf::Texture TXT_chest_closed;
	sf::Texture TXT_chest_open;

	sf::Texture TXT_menu;
	sf::Texture TXT_dead;
	sf::Texture TXT_pause;


} // End namespace