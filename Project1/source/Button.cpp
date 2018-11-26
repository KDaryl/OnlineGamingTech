#include "Button.h"

Button::Button()
{
}

Button::~Button()
{
}

void Button::update()
{
}

void Button::draw(SDL_Renderer * renderer)
{
	m_sprite.draw(renderer);
}

bool Button::clicked(InputHandler & input)
{
	return input.getMouseCollider().intersects(m_collider);
}

void Button::setTexture(ResourceHandler & resources, std::string button)
{
}
