/*
	player.h

	Declares a namespace containing useful utility functions that
	the rest of the game uses.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "global.h"

#include <SFML/Graphics.hpp>

const float C_PLAYER_SPEED = 1.5;

class player
{

	public:


		///
		/// Methods
		///

		// Reset's the player
		void init(); // position 0,0
		void init(float xpos, float ypos);

		// Handles the events
		void events(sf::Event* evnt);

		// Does logic
		void step();

		// Render's the player
		void draw();





		///
		/// Properties
		///



		// The player's position
		float x, y;

		// the player's speeds
		float xspeed, yspeed;









	private:


		///
		/// Private properties
		///

		bool keyUp, keyDown, keyLeft, keyRight;


};

#endif