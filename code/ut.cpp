/*
	ut.h
	(short for "utilities.h")

	Declares a namespace containing useful utility functions that
	the rest of the game uses.
*/

#include "ut.h"

namespace ut
{


	// Set's the seed for random number generation to the current time
	void seed()
	{
		srand((unsigned int)time(NULL));
	}


	// Generate's a random number between min and max (inclusive)
	int random(int min, int max)
	{
		return (rand()%(max+1))+min;
	}


} // End namespace