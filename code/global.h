/*
	global.h

	Declares a namespace of global varibles and functions
	that the rest of the game depend upon.
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class player;
class level;

enum GAME_STATE {S_MENU, S_PLAY, S_DEAD, S_PAUSE};

namespace global
{

	// A pointer to the RenderWindow the game uses and draws to.
	extern sf::RenderWindow* rwpWindow;

	// If this is true, the main loop is running
	extern bool bLoopRunning;

	// The state of the game
	extern GAME_STATE gsGameState;

	// A pointer to the current player object that is contained inside the gameManager
	extern player* plPlayer;
	// Pointers to the player positions
	extern float* flPlayerX;
	extern float* flPlayerY;

	// A pointer to the current level object that is contained inside the gameManager
	extern level* lvlLevel;

	// A pointer to the number of blocks the player has
	extern int* intPlayerBlocks;

	// The main font used in the game
	extern sf::Font fntMain;

	// The pause game trigger and indicator
	extern bool bGamePause;

	// The position of the exit cell (pixel coordinates)
	extern float flExitCellX;
	extern float flExitCellY;


	// The trigger to end the level and go to the next one
	extern bool blEndLevelTrigger;


	// All sound data used
	extern sf::SoundBuffer SNDBUF_mine;


} // End namespace

#endif