#pragma once
#include <vector>
#include "Sprite.h"
#include "ResourceHandler.h"
#include "InputHandler.h"
#include "CollisionCircle.h"


class Player
{
public:
	Player();
	~Player();

	void initPlayer(Vector2f pos, int textureIndex);

	void setTexture(ResourceHandler& resources);
	void setTextureRect(int textureIndex);
	void setPosition(float x, float y);

	void update(double dt); //Local player update
	void serverUpdate(); //Update from server
	void draw(SDL_Renderer* renderer);

	void handleInput(InputHandler& input);

	//getters
	//Get circle collider
	CollisionCircle& getCircleCollider() { return m_circleCollider; }
	Vector2f& getPosition() { return m_position; }

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

	std::vector<Rect> m_textureRects; //The texture rectangles of our sprite

	Vector2f m_position, m_velocity;
	float m_speed, m_deltaTime;
	int m_textureIndex;
	Sprite m_sprite;
	CollisionBox m_boxCollider;
	CollisionCircle m_circleCollider;
};

