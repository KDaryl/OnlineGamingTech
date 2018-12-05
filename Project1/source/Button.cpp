#include "Button.h"

Button::Button() :
	m_animation(true, false, .15f),
	m_collider(0,0, 540, 75)
{
	m_animation.createRectangles(15, 540, 8100, 75);
	m_sprite.setTextureRect(0, 0, 540, 75);
}

Button::~Button()
{
}

void Button::update(int dt)
{
	m_animation.update(dt);
}

void Button::draw(SDL_Renderer * renderer)
{
	m_sprite.draw(renderer);
}

void Button::checkHighlight(InputHandler & input)
{
	if (input.getMouseCollider().intersects(m_collider))
	{
		m_animation.play();
	}
	else
	{
		m_animation.playReverse();
	}
}

bool Button::clicked(InputHandler & input)
{
	return input.getMouseCollider().intersects(m_collider);
}

void Button::setTexture(ResourceHandler & resources, std::string button)
{
	m_sprite.setTexture(resources.getTexture(button));

	//Set the animation sprite
	m_animation.setSprite(m_sprite);
}
