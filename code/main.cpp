/*
	Add some sort of description here
*/


#include "global.h"
#include "ut.h"
#include "game_manager.h"
#include "player.h"
#include "level.h"

#include <iostream>


// The game manager
gameManager game;

// Clock to calculate fps
sf::Clock fpsClock;




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

	// Set the fps limit to 60
	global::rwpWindow->setFramerateLimit(60);

	// Seed the random number generator
	ut::seed();
	std::cout << "seed: " << ut::current_seed << std::endl;

	// Load the main font
	global::fntMain.loadFromFile("resources/font/pixelette.ttf");

	// Load all the sounds
	global::SNDBUF_mine.loadFromFile("resources/sound/mine.wav");

	// Start a new game
	game.newGame();




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

			// Reset game
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				game.newGame();

			game.events(&ev);

		}



		/*
			#####
			## Logic
			#####
		*/

		game.step();


		/*
			#####
			## Rendering
			#####
		*/
		ut::frameStart();
		game.draw();

		float fps;
		fps = 1.f/fpsClock.getElapsedTime().asSeconds();
		fpsClock.restart();
		sf::Text blktxt("fps: " + ut::toString(fps));
		blktxt.setFont(global::fntMain);
		blktxt.setCharacterSize(12);
		blktxt.setPosition(535,0);
		global::rwpWindow->draw(blktxt);

		ut::frameEnd();

	}


	return 0;
}