#pragma once
#include "Scene.h"
#include "Player.h"

class GameScene : Scene
{

public:
	GameScene();
	~GameScene();
	void update();
	void draw(SDL_Renderer* renderer);
	void setTexture(ResourceHandler& resources);
private:
	Player m_player;
};

