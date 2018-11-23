#pragma once
#include "SDL.h"
#include <string>
#include <map>

typedef std::map<std::string, bool> Input; //Use input as an alias for our map

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void update();

	void checkKey(std::string key, SDL_Keycode code);

	//Checks if a button has been pressed
	bool isButtonPressed(std::string button);
	//Checks if a button is being held down
	bool isButtonDown(std::string button);
private:
	Input m_current;
	Input m_previous;
	const Uint8* m_keyStates;
};

