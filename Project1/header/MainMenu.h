#pragma once
#include "Scene.h"

class MainMenuScene : public Scene
{

public:
	MainMenuScene();
	~MainMenuScene();
	void update();
	void draw(SDL_Renderer* renderer);
	void handleInput(InputHandler& input);
	void setTexture(ResourceHandler& resources);
private:
	Sprite m_playGameButton;
};
