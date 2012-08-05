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


} // End namespace