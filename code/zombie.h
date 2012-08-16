/*
	light.h

	Does nothing useful apart from make the mapCell struct usable by many other files without having to include level.h
*/

#ifndef ZOMBIE_H
#define ZOMBIE_H

/*
	zombie.h

	Declares a a zombie class that can function on it's own.
	level.h includes zombie.h
*/


#include <iostream>

#include <SFML/Graphics.hpp>

#include "global.h"
#include "ut.h"
#include "fpscalc.h"


const float C_ZOMBIE_STEP_SIZE = 0.5;



class zombie
{


	public:


		///
		/// Methods
		///


		// Resets the zombie, like new
		void reset(float xpos, float ypos, int MaxHp, int dmgStrength);

		// Updates the zombie
		void step();

		// Renders the zombie
		void draw();



		// Subtracts hp from the zombie and pushes it back from the attacker
		void damage(sf::Vector2f vecToZombie, int dmgAmmount);





		///
		/// Properties
		///

		// Position
		float x, y;

		// Speed vector
		sf::Vector2f vecSpeed;

		// Stats
		int maxHp, curHp, dmgVal;

		// Target position
		float tx, ty;

		// Vector to target
		sf::Vector2f vecToTarget;

		// Normalised vector to target
		sf::Vector2f vecToTargetNorm;

		// If the zombie is close enough to persue a target
		bool gotoTarget;





	private:

		///
		/// Private properties
		///


		sf::Clock attackTimer;

		float distanceThreshold;




		///
		/// Private methods
		///


};



#endif