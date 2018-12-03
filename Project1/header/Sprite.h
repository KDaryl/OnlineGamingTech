#pragma once
#include <iostream>
#include "SDL.h"
#include "CollisionBox.h"


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
	struct Vector2f
	{
		float x = 0;
		float y = 0;
	};

	SDL_Texture* m_texture;
	SDL_Rect m_src;
	SDL_Rect m_dst;
	Vector2f m_pos;
	CollisionBox m_collider;
};

