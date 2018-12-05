#pragma once
#include <vector>
#include "Sprite.h"
class Animation
{
public:
	Animation(bool reverse, bool loop, float length);
	~Animation();
	void setSprite(Sprite& sprite);

	void update(int dt);
	void draw(SDL_Renderer* renderer);

	void play();
	void playReverse();
	void stop();
	void createRectangles(int frames, int oneFrameW, int maxW, int height);

private:
	std::vector<SDL_Rect> m_rectangles, m_reverseRectangles;
	Sprite * m_spriteToAnimate;
	bool m_reverse, m_loop, m_finished, m_play, m_playReverse, m_animFinished;
	int m_currentRect;
	float m_length;
	int m_lpf, m_ticksGone, m_maxFrames; //Length per frame
};

