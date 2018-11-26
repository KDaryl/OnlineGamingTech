#pragma once
#include "SDL.h"
#include "ResourceHandler.h"
#include "InputHandler.h"
#include "Sprite.h"

class Scene
{
public:
	Scene() {};
	~Scene() {};
	virtual void update() = 0;
	virtual void draw(SDL_Renderer* renderer) = 0;
	virtual std::string handleInput(InputHandler& input, std::string currentScene) = 0;
	virtual void setTexture(ResourceHandler& resources) = 0;
};

