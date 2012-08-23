/*
	Add some sort of description here
*/


#include "fpscalc.h"

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

// Dead return trigger
bool deadReturn = false;



// Screen mode varibles
bool toggleScreen = false;
bool isFull = false;



// Sprites
sf::Sprite SPR_menu, SPR_dead, SPR_pause;




// The main loop function that get's called from the 'main' function
int main_loop();



///
/// main function
//
int main()
{

	// Initialize the screen
	sf::RenderWindow rw(sf::VideoMode(640,496),"Caverns");
	global::rwpWindow = &rw;

	// Seed the random number generator
	ut::seed();
	std::cout << "seed: " << ut::current_seed << std::endl;

	// Set up fpscalc
	FpsCalc::GetInstance()->Init(60);

	// Load all the fonts
	global::fntMain.loadFromFile("resources/font/pixelette.ttf");
	global::fntDeath.loadFromFile("resources/font/trebucbd.ttf");

	// Load all the sounds
	global::SNDBUF_mine.loadFromFile("resources/sound/mine.wav");
	global::SNDBUF_zombie.loadFromFile("resources/sound/zombie.wav");
	global::SNDBUF_player_hit.loadFromFile("resources/sound/player_hit.wav");
	global::SNDBUF_player_miss.loadFromFile("resources/sound/player_miss.wav");
	global::SNDBUF_player_hurt.loadFromFile("resources/sound/player_hurt.wav");
	global::SNDBUF_get_sword.loadFromFile("resources/sound/get_sword.wav");

	// Load all the textures
	global::TXT_player_down.loadFromFile("resources/texture/player_down.png");
	global::TXT_player_up.loadFromFile("resources/texture/player_up.png");
	global::TXT_player_left.loadFromFile("resources/texture/player_left.png");
	global::TXT_player_right.loadFromFile("resources/texture/player_right.png");
	global::TXT_player_down_sword.loadFromFile("resources/texture/player_down_sword.png");
	global::TXT_player_right_sword.loadFromFile("resources/texture/player_right_sword.png");

	global::TXT_wall.loadFromFile("resources/texture/wall.png");
	global::TXT_u_wall.loadFromFile("resources/texture/unbreakable_wall.png");
	global::TXT_exit.loadFromFile("resources/texture/exit.png");
	global::TXT_lantern.loadFromFile("resources/texture/lantern.png");
	global::TXT_chest_closed.loadFromFile("resources/texture/chest_closed.png");
	global::TXT_chest_open.loadFromFile("resources/texture/chest_open.png");

	global::TXT_menu.loadFromFile("resources/texture/menu.png");
	global::TXT_dead.loadFromFile("resources/texture/dead.png");
	global::TXT_pause.loadFromFile("resources/texture/paused.png");

	global::TXT_zombie_left.loadFromFile("resources/texture/zombie_left.png");
	global::TXT_zombie_right.loadFromFile("resources/texture/zombie_right.png");




	// Set sprites
	SPR_menu.setTexture(global::TXT_menu); SPR_menu.setPosition(0,0);
	SPR_dead.setTexture(global::TXT_dead); SPR_dead.setPosition(0,0);
	SPR_pause.setTexture(global::TXT_pause); SPR_pause.setPosition(0,0);




	///////           ///////
	// Enter the main loop //
	///////           ///////
	int loop_return = main_loop();




	// Close the screen
	global::rwpWindow->close();

	// Free all memmory used
	FpsCalc::GetInstance()->FreeMemory();

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

		FpsCalc::GetInstance()->SetSpeedFactor();

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
			//if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				//game.newGame();

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
				toggleScreen = true;
			else
				toggleScreen = false;


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
					if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						deadReturn = true;
					else
						deadReturn = false;
					//////////////////////////////
				break;
		}

		}



		/*
			#####
			## Logic
			#####
		*/


		////////////////////////
		// Screen

		if(toggleScreen)
		{
			toggleScreen = false;
			isFull = !isFull;

			if(isFull)
			{
				global::rwpWindow->create(sf::VideoMode::getDesktopMode(),"Caverns",sf::Style::Fullscreen);

				//const sf::View& defaultView = global::rwpWindow->getDefaultView();
				//sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
				//defaultView.SetFromRect(sf::FloatRect(0.f, 0.f, videoMode.Width, videoMode.Height));
				//defaultView.setSize(640,496);
				const sf::View dv(sf::FloatRect(0.f, 0.f, 640, 496));
				global::rwpWindow->setView(dv);

				global::rwpWindow->setMouseCursorVisible(false);
			}
			else
			{
				global::rwpWindow->create(sf::VideoMode(640,496),"Caverns");
				global::rwpWindow->setMouseCursorVisible(true);
			}
		}



		////////////////////////
		// Real

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
				if(deadReturn)
				{
					deadReturn = false;
					global::gsGameState = S_MENU;
				}
				//////////////////////////////
			break;
		}


		/*
			#####
			## Rendering
			#####
		*/

		if(global::gsGameState != S_PAUSE)
			ut::frameStart();


		sf::Text debugtxt;
		sf::Text statusTxt;
		sf::RectangleShape statusBar;

		statusTxt.setFont(global::fntMain);
		statusTxt.setCharacterSize(12);

		switch(global::gsGameState)
		{
			case S_MENU:
				//////////////////////////////
				global::rwpWindow->draw(SPR_menu);
				//////////////////////////////
			break;

			case S_PLAY:
				//////////////////////////////
				game.draw();


				statusBar.setPosition(0,480);
				statusBar.setSize(sf::Vector2f(640,16));
				statusBar.setFillColor(sf::Color(1,11,99));
				global::rwpWindow->draw(statusBar);

				statusTxt.setPosition(10,480);
				statusTxt.setString("blocks: " + ut::toString(*global::intPlayerBlocks));
				global::rwpWindow->draw(statusTxt);

				statusTxt.setPosition(130,480);
				statusTxt.setString("lanterns: " + ut::toString(*global::intPlayerLanterns));
				global::rwpWindow->draw(statusTxt);

				statusTxt.setPosition(270,480);
				statusTxt.setString("treasure: " + ut::toString(*global::intPlayerTreasure));
				global::rwpWindow->draw(statusTxt);

				statusTxt.setPosition(400,480);
				statusTxt.setString("health: " + ut::toString(global::plPlayer->getHp()) + "/100");
				global::rwpWindow->draw(statusTxt);

				statusTxt.setPosition(560,480);
				statusTxt.setString("level: " + ut::toString(game.currentLevel));
				global::rwpWindow->draw(statusTxt);
				
				
				//////////////////////////////
			break;

			case S_PAUSE:
				//////////////////////////////
				global::rwpWindow->draw(SPR_pause);
				//////////////////////////////
			break;

			case S_DEAD:
				//////////////////////////////
				global::rwpWindow->draw(SPR_dead);

				sf::Text tTxt(ut::toString(*global::intPlayerTreasure));
				tTxt.setFont(global::fntDeath);
				tTxt.setCharacterSize(20);
				tTxt.setPosition(372,265);
				tTxt.setColor(sf::Color(255,255,0));
				global::rwpWindow->draw(tTxt);

				sf::Text lTxt(ut::toString(*global::ptrCurLevel));
				lTxt.setFont(global::fntDeath);
				lTxt.setCharacterSize(20);
				lTxt.setPosition(234,265);
				lTxt.setColor(sf::Color(255,255,0));
				global::rwpWindow->draw(lTxt);
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