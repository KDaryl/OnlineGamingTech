#include "Player.h"

Player::Player()
{

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

void Player::update()
{
}

void Player::draw(SDL_Renderer * renderer)
{
	//Draw our player
	m_sprite.draw(renderer);
}