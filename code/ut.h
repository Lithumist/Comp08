/*
	ut.h
	(short for "utilities.h")

	Declares a namespace containing useful utility functions that
	the rest of the game uses.
*/

#ifndef UT_H
#define UT_H

#include <stdlib.h>
#include <time.h>

namespace ut
{

	// Set's the seed for random number generation to the current time
	void seed();

	// Generate's a random number between min and max (inclusive)
	int random(int min, int max);


} // End namespace

#endif