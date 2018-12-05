#pragma once
#include "ResourceHandler.h"
#include "InputHandler.h"
#include "Sprite.h"
#include "CollisionBox.h"
#include "Animation.h"
class Button
{
public:
	Button(int x, int y);
	~Button();
	void update(int dt);
	void draw(SDL_Renderer* renderer);

	void checkHighlight(InputHandler& input); //Checks if the mouse is hovering over the button

	bool clicked(InputHandler& input);
	void setTexture(ResourceHandler& resources, std::string button);
	CollisionBox& getCollider() { return m_collider; }
private:
	Sprite m_sprite;
	Animation m_animation;
	CollisionBox m_collider; //Our buttons collision box
	typedef std::pair<float, float> Vector2f;
};

