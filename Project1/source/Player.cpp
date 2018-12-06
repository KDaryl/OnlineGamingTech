#include "Player.h"

Player::Player()
{
	//Add the four rectangles for our player spritesheet
	m_textureRects.push_back(Rect(0, 0, 75, 75));
	m_textureRects.push_back(Rect(75, 0, 75, 75));
	m_textureRects.push_back(Rect(150, 0, 75, 75));
	m_textureRects.push_back(Rect(225, 0, 75, 75));
}

Player::~Player()
{
}

void Player::setTexture(ResourceHandler & resources)
{
	m_sprite.setTexture(resources.getTexture("Player"));
	//Set the sprite to the blue circle in the texture
	m_sprite.setTextureRect(75, 0, 75, 75);
}

void Player::setTextureRect(int textureIndex)
{
	auto rect = m_textureRects.at(textureIndex); //Get the new textureRect
	m_sprite.setTextureRect(rect.m_x, rect.m_y, rect.m_w, rect.m_h);
}

void Player::setPosition(float x, float y)
{
	//Set our starting position
	m_position.x = x;
	m_position.y = y;
}

void Player::update()
{
}

void Player::serverUpdate()
{
}

void Player::draw(SDL_Renderer * renderer)
{
	//Draw our player
	m_sprite.draw(renderer);
}

void Player::handleInput(InputHandler & input)
{
}
