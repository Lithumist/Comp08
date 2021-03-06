/*
	game_manager.h

	Declares a class that manages the gameplay.
*/

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "ut.h"
#include "global.h"
#include "player.h"
#include "level.h"

class gameManager
{

	private:

		///
		/// Private properties
		///

		// Pasue trigger
		bool doPause;

		// Light trigger
		bool toggleLight;

	public:


		///
		/// Methods
		///


		// Terminates the current game and starts a new one
		void newGame();

		// Handles events
		void events(sf::Event* evnt);

		// Does logic
		void step();

		// Renders the game
		void draw();


		// Resets the game, but going to the next level
		void nextLevel();






		///
		/// Properties
		///

		player gamePlayer;
		level gameLevel;

		int currentLevel;


};

#endif