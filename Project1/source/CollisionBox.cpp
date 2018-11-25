#include "CollisionBox.h"

CollisionBox::CollisionBox() :
	m_x(0),
	m_y(0),
	m_w(0),
	m_h(0)
{
}

CollisionBox::CollisionBox(float x, float y, float w, float h) :
	m_x(x),
	m_y(y),
	m_w(w),
	m_h(h)
{
}

CollisionBox::~CollisionBox()
{
}

bool CollisionBox::intersects(CollisionBox & other)
{
	//Checks for rectangular collision
	if (m_x < other.w() && m_w > other.x() &&
		m_y < other.h() && other.h() > m_y)
		return false;
	return true;
}

void CollisionBox::setSize(float w, float h)
{
	m_w = w;
	m_h = h;
}

void CollisionBox::setSize(float x, float y, float w, float h)
{
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
}

void CollisionBox::setPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}
