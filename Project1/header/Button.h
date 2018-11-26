#pragma once
#include "ResourceHandler.h"
#include "InputHandler.h"
#include "Sprite.h"
#include "CollisionBox.h"
class Button
{
public:
	Button();
	~Button();
	void update();
	void draw(SDL_Renderer* renderer);

	bool clicked(InputHandler& input);
	void setTexture(ResourceHandler& resources, std::string button);
	CollisionBox& getCollider() { return m_collider; }
private:
	Sprite m_sprite; //-> this needs to be an animation
	CollisionBox m_collider; //Our buttons collision box
};

