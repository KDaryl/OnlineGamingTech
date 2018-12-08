#include "CollisionCircle.h"

CollisionCircle::CollisionCircle() :
	m_x(0),
	m_y(0),
	m_r(0)
{
}

CollisionCircle::CollisionCircle(float x, float y, float r) :
	m_x(x),
	m_y(y),
	m_r(r)
{

}

CollisionCircle::~CollisionCircle()
{
}

bool CollisionCircle::intersects(CollisionCircle & other)
{
	if(powf((other.x() - m_x), 2) + powf((m_y - other.y()), 2) <= powf((m_r - other.r()), 2))
		return true;
	return false;
}

void CollisionCircle::setRadius(float r)
{
	m_r = r;
}

void CollisionCircle::setSize(float x, float y, float r)
{
	m_x = x;
	m_y = y;
	m_r = r;
}

void CollisionCircle::setPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}
