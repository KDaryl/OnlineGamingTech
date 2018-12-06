#pragma once
#include <vector>
#include "Sprite.h"
#include "ResourceHandler.h"
#include "InputHandler.h"

class Player
{
public:
	Player();
	~Player();

	void setTexture(ResourceHandler& resources);
	void setTextureRect(int textureIndex);
	void setPosition(float x, float y);

	void update(); //Local player update
	void serverUpdate(); //Update from server
	void draw(SDL_Renderer* renderer);

	void handleInput(InputHandler& input);
private:
	//Simple rect class
	struct Rect
	{
		Rect(int x, int y, int w, int h) :
			m_x(x),
			m_y(y),
			m_h(h),
			m_w(w)
		{

		}
		int m_x, m_y, m_w, m_h;
	};
	//Simple Vector 2f class for position manipulation
	struct Vector2f
	{
		Vector2f() {}
		Vector2f(float _x, float _y) :
			x(_x),
			y(_y)
		{}

		float x, y;
	};

	std::vector<Rect> m_textureRects; //The texture rectangles of our sprite

	Vector2f m_position;
	Sprite m_sprite;
};

