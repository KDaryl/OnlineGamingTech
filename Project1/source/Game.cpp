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

				//m_sceneManager.setCurrent("Game Scene");

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
		Uint64 NOW = SDL_GetPerformanceCounter();
		Uint64 LAST = 0;
		double deltaTime = 0;

		//Event handler
		SDL_Event e;

		//While we are not quiting
		while (!m_quitGame)
		{
			LAST = NOW;
			NOW = SDL_GetPerformanceCounter();

			//Calculate DT
			deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

			//Process any events
			processEvents(e);

			//If we are lagging behind our target framerate then lets do some extra updates
			if (deltaTime >= m_msPerFrame)
			{
				update(deltaTime);
				deltaTime -= m_msPerFrame;
			}
			update(deltaTime);
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


void Game::update(double dt)
{
	if (m_serverConnection.lostConnecion())
	{
		m_connectedToServer = false;
	}

	if (m_gameHasStarted == false)
	{
		//Set start game true for thehost so the host can setup game parameters
		if (m_serverConnection.selectedAsHost())
		{
			//Setup game parameters and send them to the other player and then start the game
			setUpGame();
			m_startGame = true;
		}

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
	m_sceneManager.update(dt);

	//For joining the server
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
			m_connectedToServer = true;
			std::cout << "Connected" << std::endl;
		}

		m_clickedJoin = false;
	}
}

//This method is called from the host, in here we will set the position and colour of the host
//and the other players, for now we are only doing this for 1 other player
void Game::setUpGame()
{
	int hostColourIndex = randInt(0, 3);
	int hostPosition = randInt(0, 3);

	//Set the hosts position
	m_myColourInt = hostColourIndex;
	m_mystartPosition = hostPosition;

	//Set the color and position of the other player(s)
	int otherStartPos = hostPosition == 0 ? 3 : hostPosition == 1 ? 2 : hostPosition == 2 ? 1 : 0;
	int otherColour = hostColourIndex == 0 ? 3 : hostColourIndex == 1 ? 2 : hostColourIndex == 2 ? 1 : 0;

	std::string msg = "";
	msg = "Colour:" + std::to_string(otherColour) + ",Position:" + std::to_string(otherStartPos);

	//Send the message to every other player (just 1 for now) as a SetupGame packet
	//m_serverConnection.SendString(msg, P_ChatMessage);

	std::cout << "Setting up game" << std::endl;
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
