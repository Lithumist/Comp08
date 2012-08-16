/*
	ut.h
	(short for "utilities.h")

	Declares a namespace containing useful utility functions that
	the rest of the game uses.
*/

#ifndef UT_H
#define UT_H

#include "global.h"

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>

namespace ut
{

	// Sets the seed for random number generation to the current time
	void seed();
	// The current seed (do not modify! )
	extern unsigned int current_seed;

	// Generates a random number between min and max (inclusive)
	int random(int min, int max);



	// Clears screen ready for drawing next frame
	void frameStart();

	// Flips the screen buffers
	void frameEnd();




	// Rounds a float accurately to an integer
	int round(float number);




	// Set of functions to convert numbers to strings
	std::string toString(int num);
	std::string toString(float num);




	// Returns a vector of coordinates that make up a line between 2 points
	// USES TILE COORDINATES!
	std::vector<sf::Vector2i> calculateLine(int x0, int y0, int x1, int y1);




	// Returns the distance between 2 points
	float distanceBetween(float x1, float y1, float x2, float y2);


} // End namespace

#endif