/*
	global.h

	Declares a namespace of global varibles and functions
	that the rest of the game depend upon.
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <SFML/Graphics.hpp>

class player;
class level;

namespace global
{

	// A pointer to the RenderWindow the game uses and draws to.
	extern sf::RenderWindow* rwpWindow;

	// If this is true, the main loop is running
	extern bool bLoopRunning;

	// A pointer to the current player object that is contained inside the gameManager
	extern player* plPlayer;

	// A pointer to the current level object that is contained inside the gameManager
	extern level* lvlLevel;


} // End namespace

#endif