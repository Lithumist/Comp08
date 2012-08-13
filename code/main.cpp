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



// Menu start game trigger
bool menuStart = false;

// Pause return trigger
bool pauseReturn = false;




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


			switch(global::gsGameState)
			{
				case S_MENU:
					//////////////////////////////
					if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
						menuStart = true;
					else
						menuStart = false;
					//////////////////////////////
				break;

				case S_PLAY:
					//////////////////////////////
					game.events(&ev);
					//////////////////////////////
				break;

				case S_PAUSE:
					//////////////////////////////
					if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						pauseReturn = true;
					else
						pauseReturn = false;
					//////////////////////////////
				break;

				case S_DEAD:
					//////////////////////////////
					//////////////////////////////
				break;
		}

		}



		/*
			#####
			## Logic
			#####
		*/
		switch(global::gsGameState)
		{
			case S_MENU:
				//////////////////////////////
				if(menuStart)
				{
					menuStart = false;
					global::gsGameState = S_PLAY;
					game.newGame();
				}
				//////////////////////////////
			break;

			case S_PLAY:
				//////////////////////////////
				game.step();
				//////////////////////////////
			break;

			case S_PAUSE:
				//////////////////////////////
				if(pauseReturn)
				{
					global::bGamePause = false;
					pauseReturn = false;
					global::gsGameState = S_PLAY;
				}
				//////////////////////////////
			break;

			case S_DEAD:
				//////////////////////////////
				//////////////////////////////
			break;
		}


		/*
			#####
			## Rendering
			#####
		*/
		ut::frameStart();

		switch(global::gsGameState)
		{
			case S_MENU:
				//////////////////////////////
				//////////////////////////////
			break;

			case S_PLAY:
				//////////////////////////////
				game.draw();
				//////////////////////////////
			break;

			case S_PAUSE:
				//////////////////////////////
				//////////////////////////////
			break;

			case S_DEAD:
				//////////////////////////////
				//////////////////////////////
			break;
		}

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