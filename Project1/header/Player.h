#pragma once
#include "Sprite.h"
#include "ResourceHandler.h"

class Player
{
public:
	Player();
	~Player();

	void setTexture(ResourceHandler& resources);

	void update();
	void draw(SDL_Renderer* renderer);
private:
	//TO DO: Create vector of SDL_Rect for easily getting the rectangle for a circle in the player spritsheet

	Sprite m_sprite;
};

