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

	// Pointers to the number of blocks, lanterns and treasure the player has
	extern int* intPlayerBlocks;
	extern int* intPlayerLanterns;
	extern int* intPlayerTreasure;

	// Player self harm flag and co
	extern bool zombieHitPlayerFlag;
	extern float zombieX;
	extern float zombieY;
	extern int zombieDmg;

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



	// All texture data used
	extern sf::Texture TXT_player_down;
	extern sf::Texture TXT_player_up;
	extern sf::Texture TXT_player_left;
	extern sf::Texture TXT_player_right;

	extern sf::Texture TXT_wall;
	extern sf::Texture TXT_u_wall;
	extern sf::Texture TXT_exit;
	extern sf::Texture TXT_lantern;
	extern sf::Texture TXT_chest_closed;
	extern sf::Texture TXT_chest_open;

	extern sf::Texture TXT_menu;
	extern sf::Texture TXT_dead;
	extern sf::Texture TXT_pause;


} // End namespace

#endif