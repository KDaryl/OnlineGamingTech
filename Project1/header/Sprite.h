#pragma once
#include <iostream>
#include "SDL.h"
#include <random>
#include "CollisionBox.h"
#include "Vector2f.h"

static int randInt(int min, int max)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(min, max); //Ste min and max values for random number
	//Return a random value between min and max
	return dis(gen);
}

///
/// Simple sprite class with rectangular collider, can change this to circle collider with base class "Collider"
///
class Sprite
{
public:
	Sprite();
	~Sprite();

	void draw(SDL_Renderer* renderer);
	void draw(SDL_Renderer* renderer, SDL_Rect* src);

	//Setters
	void setTexture(SDL_Texture* texture);
	void setPosition(float x, float y);
	void setTextureRect(int x, int y, int w, int h);

	//Getters
	float getX() { return m_pos.x; }
	float getY() { return m_pos.y; }
	float getW() { return m_dst.w; }
	float getH() { return m_dst.h; }
	SDL_Rect getRect() { return m_dst; }
	CollisionBox& getCollider() { return m_collider; }
private:
	SDL_Texture* m_texture;
	SDL_Rect m_src;
	SDL_Rect m_dst;
	Vector2f m_pos;
	CollisionBox m_collider;
};

