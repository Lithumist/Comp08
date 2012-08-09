/*
	ut.h
	(short for "utilities.h")

	Declares a namespace containing useful utility functions that
	the rest of the game uses.
*/

#include "ut.h"

namespace ut
{


	// Sets the seed for random number generation to the current time
	void seed()
	{
		current_seed = (unsigned int)time(NULL);
		time_t lol = time(NULL);
		srand(current_seed);
	}
	// The current seed (do not modify! )
	unsigned int current_seed = 0;


	// Generates a random number between min and max (inclusive)
	int random(int min, int max)
	{
		return (rand()%(max+1))+min;
	}





	// Clears screen ready for drawing next frame
	void frameStart()
	{
		global::rwpWindow->clear();
	}

	// Flips the screen buffers
	void frameEnd()
	{
		global::rwpWindow->display();
	}






	// Rounds a float accurately to an integer
	int round(float number)
	{
		return (int)(floor(number+0.5));
	}


} // End namespace