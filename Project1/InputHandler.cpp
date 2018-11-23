#include "InputHandler.h"

InputHandler::InputHandler()
{
	//Keep a pointer to the keyboard state
	m_keyStates = SDL_GetKeyboardState(NULL);

	//Add buttons to our input handler
	m_current["W"] = false;
	m_current["A"] = false;
	m_current["S"] = false;
	m_current["D"] = false;
	
	//Copy the values of current to previous
	m_previous = m_current;
}

InputHandler::~InputHandler()
{
}

void InputHandler::update()
{
	SDL_PumpEvents(); //Update the keystate

	m_previous = m_current; //Set our previous

	//Check if the keys are currently down
	m_current["W"] = m_keyStates[SDL_SCANCODE_W];
	m_current["A"] = m_keyStates[SDL_SCANCODE_A];
	m_current["S"] = m_keyStates[SDL_SCANCODE_S];
	m_current["D"] = m_keyStates[SDL_SCANCODE_D];
}

void InputHandler::checkKey(std::string key, SDL_Keycode code)
{
	m_current[key] = m_keyStates[code];
}

bool InputHandler::isButtonPressed(std::string button)
{
	return m_current[button] && !m_previous[button] ? true : false;
}

bool InputHandler::isButtonDown(std::string button)
{
	return m_current[button];
}
