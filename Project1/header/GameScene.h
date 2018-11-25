#pragma once
#include "Scene.h"
#include "Player.h"

class GameScene : public Scene
{

public:
	GameScene();
	~GameScene();
	void update();
	void draw(SDL_Renderer* renderer);
	void handleInput(InputHandler& input);
	void setTexture(ResourceHandler& resources);
private:
	Player m_player;
	Sprite m_bgSprite;
};

