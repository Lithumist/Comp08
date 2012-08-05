/*
	Add some sort of description here
*/


#include "global.h"
#include "ut.h"

#include <iostream>



// The main loop function that get's called from the 'main' function
int main_loop();



///
/// main function
//
int main()
{

	// Initialize the screen
	sf::RenderWindow rw(sf::VideoMode(640,480),"Comp08");
	global::rwpWindow = &rw;

	// Seed the random number generator
	ut::seed();




	///////           ///////
	// Enter the main loop //
	///////           ///////
	int loop_return = main_loop();




	// Close the screen
	global::rwpWindow->close();

	return 0;
}















///
/// main_loop function
///
int main_loop()
{

	// Make sure that the loop will run
	global::bLoopRunning = true;


	while(global::bLoopRunning)
	{

		/*
			#####
			## Events
			#####
		*/

		sf::Event ev;
		while(global::rwpWindow->pollEvent(ev))
		{
			// window closed
			if(ev.type == sf::Event::Closed)
				global::bLoopRunning = false;
		}



		/*
			#####
			## Logic
			#####
		*/


		/*
			#####
			## Rendering
			#####
		*/

	}


	return 0;
}