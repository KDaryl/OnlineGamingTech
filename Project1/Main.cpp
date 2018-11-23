#include "Game.h"

int main(int argc, char* args[])
{
	//Create the game with a window of the size specified in Width and Height
	//and with the FPS
	Game game(60, 1280, 720);

	//Run the game
	game.run();

	return 0;
} 