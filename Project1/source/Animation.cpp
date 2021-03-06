#include "Animation.h"

Animation::Animation( bool reverse, bool loop, float length):
	m_reverse(reverse),
	m_loop(loop),
	m_finished(false),
	m_play(false),
	m_playReverse(false),
	m_length(length),
	m_lpf(0),
	m_animFinished(false),
	m_ticksGone(0),
	m_currentRect(0)
{
}

Animation::~Animation()
{
}

void Animation::setSprite(Sprite& sprite)
{
	m_spriteToAnimate = &sprite;
}

void Animation::update(int dt)
{
	//If playing, update the animation
	if ((m_play || m_playReverse) && m_animFinished == false)
	{
		m_ticksGone += dt; //Add to our ticks gone

		//If we have passed our ticks perframe
		if (m_ticksGone >= m_lpf)
		{
			m_ticksGone -= m_lpf;

			//Increase the frame
			m_currentRect++;

			//If we have reached the max frames
			if (m_currentRect == m_maxFrames)
			{
				//Go back to our previous frame
				m_currentRect--;
				m_ticksGone = 0;
				m_animFinished = true;
			}

			//Set sprite texture
			if(!m_playReverse)
				m_spriteToAnimate->setTextureRect(m_rectangles.at(m_currentRect).x, m_rectangles.at(m_currentRect).y, m_rectangles.at(m_currentRect).w, m_rectangles.at(m_currentRect).h);
			else
				m_spriteToAnimate->setTextureRect(m_reverseRectangles.at(m_currentRect).x, m_reverseRectangles.at(m_currentRect).y, m_reverseRectangles.at(m_currentRect).w, m_reverseRectangles.at(m_currentRect).h);
		}
	}
}

void Animation::draw(SDL_Renderer * renderer)
{
	//Draw the sprite
	if(m_play && !m_playReverse )
		m_spriteToAnimate->draw(renderer, &m_rectangles[m_currentRect]);
	else if(m_playReverse)
		m_spriteToAnimate->draw(renderer, &m_reverseRectangles[m_currentRect]);
}

void Animation::play()
{
	if (m_play == false)
	{
		//Reset our variables
		m_play = true;
		m_playReverse = false;
		m_currentRect = 0;
		m_ticksGone = 0;
		m_animFinished = false;
	}
}

void Animation::playReverse()
{
	if (m_playReverse == false)
	{
		m_play = false;
		m_playReverse = true;
		m_currentRect = 0;
		m_ticksGone = 0;
		m_animFinished = false;
	}
}

void Animation::stop()
{
	m_play = false;
	m_playReverse = false;
}

//Creates the frames for an animation
void Animation::createRectangles(int frames, int oneFrameW, int maxW, int height)
{
	//Setup our frames for our rectangle and determien the length per frame
	m_lpf = (m_length * 1000) / frames;

	for (int i = 0; i < frames; i++)
	{
		//Create rectangle
		SDL_Rect rect;
		rect.x = oneFrameW * i;
		rect.y = 0;
		rect.w = oneFrameW;
		rect.h = height;
		m_rectangles.push_back(rect);
	}

	//Set our max frames
	m_maxFrames = m_rectangles.size();

	//If our animation can reverse, create our reverse rectangles
	if (m_reverse)
	{
		m_reverseRectangles = m_rectangles; 
		std::reverse(m_reverseRectangles.begin(), m_reverseRectangles.end());
	}
}
