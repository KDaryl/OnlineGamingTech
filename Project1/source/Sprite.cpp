#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::draw(SDL_Renderer* renderer)
{
	m_dst.x = (int)m_pos.x;
	m_dst.y = (int)m_pos.y;

	SDL_RenderCopy(renderer, m_texture, &m_src, &m_dst);
}

void Sprite::setTexture(SDL_Texture* texture)
{
	m_texture = texture;

	//Get the width and height of the texture
	SDL_QueryTexture(m_texture, NULL, NULL, &m_src.w, &m_src.h);

	//Assign destination rectangle the source rectangle as we want to draw the entire thing
	m_dst = m_src;
}

void Sprite::setPosition(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;
}

void Sprite::setTextureRect(int x, int y, int w, int h)
{
	//Change the source rectangle of the image we want to display
	m_src.x = x;
	m_src.y = y;
	m_src.w = w;
	m_src.h = h;

	//Set our destination rectangle to have the same W and H
	m_dst.w = w;
	m_dst.h = h;
}
