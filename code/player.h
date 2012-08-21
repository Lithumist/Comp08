/*
	player.h

	Declares a namespace containing useful utility functions that
	the rest of the game uses.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "global.h"
#include "ut.h"
#include "level.h"
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "fpscalc.h"

const float C_PLAYER_SPEED = 1.5;
const int C_PLAYER_DAMAGE = 1;
const float C_ATTACK_TIME_DELAY = 1; // seconds
extern float PLAYER_MINE_TIME;

class player
{

	public:


		///
		/// Methods
		///

		// Reset's the player
		void init(); // position 0,0
		void init(float xpos, float ypos);
		void initNextLevel(float xpos, float ypos);

		// Handles the events
		void events(sf::Event* evnt);

		// Does logic
		void step();

		// Render's the player
		void draw();

		// Kills the player instantly
		void kill();


		// Hurts the player
		void hurt(int damage);
		// Heals the player
		void heal(int ammount);
		// Returns the current HP
		int getHp();





		///
		/// Properties
		///



		// The player's position
		float x, y;

		// the Player's speeds
		float xspeed, yspeed;









	private:


		///
		/// Private properties
		///

		bool keyUp, keyDown, keyLeft, keyRight;

		bool rmCell, placeCell, rmLantern, placeLantern;
		bool fastMine;

		bool hasReset;
		sf::Clock cellTime;

		bool attack;
		sf::Clock attackTime;

		sf::IntRect cellPlayer, cellLeft, cellRight, cellUp, cellDown, cellTopLeft, cellTopRight, cellBottomLeft, cellBottomRight, liveRectPlayer;

		int dir;
		// 1 up
		// 2 right
		// 3 down
		// 4 left

		int numberBlocks, numberLanterns, numberTreasure;

		std::vector<sf::Vector2f> collisionTestList;

		sf::Sound SND_mine;

		sf::Sprite SPR_up, SPR_down, SPR_left, SPR_right;

		// The player's HP (out of 100)
		int hp;






		///
		/// Private methods
		///

		void handleCollisions();

};

#endif