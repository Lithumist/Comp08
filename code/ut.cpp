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
		srand((unsigned int)time(NULL));
	}


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


} // End namespace