#pragma once
#include <iostream>
class CollisionCircle
{
public:
	CollisionCircle();
	CollisionCircle(float x, float y, float r);
	~CollisionCircle();

	bool intersects(CollisionCircle& other);
	void setRadius(float r);
	void setSize(float x, float y, float r);
	void setPosition(float x, float y);
	float x() { return m_x; }
	float y() { return m_y; }
	float r() { return m_r; }
private:
	float m_x, m_y, m_r;
};

