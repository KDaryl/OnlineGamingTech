#include "Game.h"

Game::Game(Uint32 FPS, Uint32 windoWidth, Uint32 windowHeight) :
	m_gWindow(NULL),
	m_gScreenSurface(NULL),
	m_quitGame(false),
	m_gameCreated(true),
	m_connectedToServer(false),
	m_clickedJoin(false),
	m_startGame(false),
	m_frameRate(FPS),
	m_msPerFrame(0),
	m_resources("images/"),
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
		m_gWindow = SDL_CreateWindow("OGT Project - Daryl Keogh C00207452", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windoWidth, windowHeight, SDL_WINDOW_SHOWN);

		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;

		//Initialise loading of PNG's
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			m_gameCreated = false;
		}	
		else if (m_gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			m_gameCreated = false;
		}

		else
		{	
			m_gRenderer = SDL_CreateRenderer(m_gWindow, -1, SDL_RENDERER_ACCELERATED);
			
			if (m_gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				m_gameCreated = false;
			}
			else
			{
				//Get window surface
				m_gScreenSurface = SDL_GetWindowSurface(m_gWindow);
				m_msPerFrame = (1.0 / m_frameRate) * 1000; //Get the MS per frame

				//Load our textures
				m_resources.loadTextures(*m_gRenderer);

				//Set all the textures for the game as appropriate
				m_sceneManager.setTexture(m_resources);

				//Set connected to server to false
				m_connectedToServer = false;
			}
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
	if (m_serverConnection.lostConnecion())
	{
		m_connectedToServer = false;
	}

	if (m_gameHasStarted == false)
	{
		//Set start game true for thehost so the host can setup game parameters
		if (m_serverConnection.selectedAsHost())

		//set player colours and starting positions and send them to the other player
		if (m_startGame)
		{
			m_gameHasStarted = true; //Set game started to true
			m_sceneManager.setCurrent("Game Scene");
		}
	}
	//Handle input
	handleInput();

	//Update Our sceneManager
	m_sceneManager.update();

	//Dont delete, this is currently not reachable but will be used for future reference
	if (!m_connectedToServer && m_clickedJoin)
	{

		//Try connect to our server, if it fails output the error
		if (!m_serverConnection.ConnectToServer())
		{
			std::cout << "Could not connect to central server" << std::endl;
		}
		//Else say we connected and set our boolean
		else
		{
			m_clickedJoin = false;
			m_connectedToServer = true;
			std::cout << "Connected" << std::endl;
		}
	}
}

void Game::handleInput()
{
	//Update the input handler
	m_input.update();

	//Handle input in our scene manager and get the return action
	std::string action = m_sceneManager.handleInput(m_input);

	//Handle actions returned
	if (action == "Join Game")
		m_clickedJoin = true;
	else if (action == "Exit Game")
		m_quitGame = true;
}

void Game::draw()
{
	//Clear the entire screen
	SDL_RenderClear(m_gRenderer);

	//Draw our game objects here
	m_sceneManager.draw(m_gRenderer);

	//Update the surface
	SDL_RenderPresent(m_gRenderer);
}

void Game::close()
{
	//Destroy any loaded textures
	m_resources.destroyTextures();

	//Destroy window
	SDL_DestroyWindow(m_gWindow);
	m_gWindow = NULL;

	//Quit SDL image systems
	IMG_Quit();

	//Quit SDL subsystems
	SDL_Quit();
}
