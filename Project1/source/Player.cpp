#include "Player.h"

Player::Player() :
	m_speed(0.5f),
	m_deltaTime(0.0f)
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

void Player::initPlayer(Vector2f pos, int textureIndex)
{
	m_position = pos;
	m_textureIndex = textureIndex;
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

void Player::update(double dt)
{ 
	m_deltaTime = dt; //Set our delta time

	//Add our velocity to our position
	m_position += m_velocity;


	m_sprite.setPosition(m_position.x, m_position.y);
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
	//Reset velocity
	m_velocity.zeroVector();

	//Add speed to our position if we move
	if(input.isButtonDown("W"))
	{
		m_velocity.y = -1;
	}
	if (input.isButtonDown("S"))
	{
		m_velocity.y = 1;
	}
	if (input.isButtonDown("A"))
	{
		m_velocity.x = -1;
	}
	if (input.isButtonDown("D"))
	{
		m_velocity.x = 1;
	}

	//Get the speed of the vector in both x and y
	if (m_velocity != Vector2f(0, 0))
	{
		//Set the velocity by getting the angle
		m_velocity = m_velocity.normalise() * m_speed * m_deltaTime;
		std::cout << m_velocity << std::endl;
	}
}
