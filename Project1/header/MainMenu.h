#pragma once
#include "Scene.h"
#include "Button.h"
#include "Timer.h"
#include <map>

class MainMenuScene : public Scene
{

public:
	MainMenuScene();
	~MainMenuScene();
	void init(bool isHostBool);
	void update();
	void draw(SDL_Renderer* renderer);
	std::string handleInput(InputHandler& input, std::string currentScene);
	void setTexture(ResourceHandler& resources);
private:
	std::map<std::string, Button*> m_buttons;
	Timer m_timer;
};
