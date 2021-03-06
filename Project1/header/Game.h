#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <random>

//Include MY header files
#include "Timer.h"
#include "SceneManager.h"

class Game
{
public:
	Game(Uint32 FPS, Uint32 windoWidth, Uint32 windowHeight);
	~Game() {};
	void run(); //Here is where are game loop will be
	void processEvents(SDL_Event& e); //Process Game events
	void update(double dt); //Where we will update the game
	void setUpGame(); //Where we setup the game parameters
	void handleInput(); //Where we will update and handle input
	void draw(); //Where we will draw the game
	void close(); //Where we will close/exit our game

	bool& gameCreated() { return m_gameCreated; }
private:
	//The window we'll be rendering to
	SDL_Window* m_gWindow;
	//The surface contained by the window
	SDL_Surface* m_gScreenSurface;
	//The renderer, we will use this to draw images
	SDL_Renderer* m_gRenderer;
	bool m_gameCreated; // Bool to hold wheter the game was succesfulyl created or not
	bool m_quitGame; //Bool to hold wheter to quit the game or not

	//Framerate control
	int m_frameRate;
	int m_msPerFrame;

	//Game variables
	InputHandler m_input;
	SceneManager m_sceneManager;
	ResourceHandler m_resources;
	 
	//variables for connection and sendign updates
	bool m_connectedToServer;
	bool m_clickedJoin;
	bool m_startGame, m_gameHasStarted;

	//Packet sending/recieving objects
	int m_myColourInt, m_mystartPosition;
};