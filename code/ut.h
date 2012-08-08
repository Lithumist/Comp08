/*
	ut.h
	(short for "utilities.h")

	Declares a namespace containing useful utility functions that
	the rest of the game uses.
*/

#ifndef UT_H
#define UT_H

#include "global.h"

#include <stdlib.h>
#include <time.h>

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


} // End namespace

#endif