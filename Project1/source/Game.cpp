#include "Game.h"

Game::Game(Uint32 FPS, Uint32 windoWidth, Uint32 windowHeight) :
	m_gWindow(NULL),
	m_gScreenSurface(NULL),
	m_quitGame(false),
	m_gameCreated(true),
	m_frameRate(FPS),
	m_msPerFrame(0),
	m_serverConnection("127.0.0.1", 100) //Create a connection to the server at IP and PORT
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		m_gameCreated = false;
	}
	else
	{
		//Create window
		m_gWindow = SDL_CreateWindow("Entity Component System", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windoWidth, windowHeight, SDL_WINDOW_SHOWN);
		if (m_gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			m_gameCreated = false;
		}
		else
		{
			//Get window surface
			m_gScreenSurface = SDL_GetWindowSurface(m_gWindow);
			m_msPerFrame = (1.0 / m_frameRate) * 1000; //Get the MS per frame

			m_connectedToServer = false;
		}
	}
}

void Game::run()
{
	if (m_gameCreated)
	{
		//Create our timer
		Timer timer;
		int lag = 0; //Lag will determine if we need to catch up with updates

		//Event handler
		SDL_Event e;

		//While we are not quiting
		while (!m_quitGame)
		{
			//Start our timer
			timer.start();
			lag += timer.getTicks(); //Get the time gone since we last restarted

			//Process any events
			processEvents(e);

			//If we are lagging behind our target framerate then let sdo some extra updates
			if (lag > m_msPerFrame)
			{
				update();
				lag -= m_msPerFrame;
			}
			update();
			draw();
		}
	}

	//Close the gaame
	close();
}

void Game::processEvents(SDL_Event & e)
{
	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests to quit
		if (e.type == SDL_QUIT)
		{
			//Set quit to true
			m_quitGame = true;
		}
	}
}

void Game::update()
{
	m_input.update(); //Update the input handler

	if (!m_connectedToServer && m_input.isButtonPressed("W"))
	{

		//If it fails to connect to the server then delete our ptr
		if (!m_serverConnection.ConnectToServer())
		{
			std::cout << "Could not connect to central server" << std::endl;
		}
		else
		{
			m_connectedToServer = true;
			std::cout << "Connected" << std::endl;
		}
	}
}

void Game::draw()
{
	//Clear the entire screen
	SDL_FillRect(m_gScreenSurface, NULL, 0x000000);

	//Draw our game objects here

	//Update the surface
	SDL_UpdateWindowSurface(m_gWindow);
}

void Game::close()
{
	//Destroy window
	SDL_DestroyWindow(m_gWindow);
	m_gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
