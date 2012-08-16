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





	// Set of functions to convert numbers to strings
	std::string toString(int num){std::stringstream ss; ss << num; return ss.str();}
	std::string toString(float num){std::stringstream ss; ss << num; return ss.str();}




	// Returns a vector of coordinates that make up a line between 2 points
	// USES TILE COORDINATES!
	// http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm
	// Slight modifications
	std::vector<sf::Vector2i> calculateLine(int x0, int y0, int x1, int y1)
	{

		std::vector<sf::Vector2i> tmp;

		int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
		int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
		int err = (dx>dy ? dx : -dy)/2, e2;
 
		for(;;){
			tmp.push_back(sf::Vector2i(x0,y0));

			if (x0==x1 && y0==y1) break;
			e2 = err;
			if (e2 >-dx) { err -= dy; x0 += sx; }
			if (e2 < dy) { err += dx; y0 += sy; }
		}

		return tmp;

	}



	// Returns the distance between 2 points
	float distanceBetween(float x1, float y1, float x2, float y2)
	{
		float d1 = x1 - x2;
		float d2 = y1 - y2;
		return sqrt(d1*d1 + d2*d2);
	}


} // End namespace