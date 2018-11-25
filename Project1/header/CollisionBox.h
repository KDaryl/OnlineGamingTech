#pragma once
class CollisionBox
{
public:
	CollisionBox();
	CollisionBox(float x, float y, float w, float h);
	~CollisionBox();

	bool intersects(CollisionBox& other);
	void setSize(float w, float h);
	void setSize(float x, float y, float w, float h);
	void setPosition(float x, float y);
	float x() { return m_x; }
	float y() { return m_y; }
	float w() { return m_w; }
	float h() { return m_h; }
private:
	float m_x, m_y, m_w, m_h;
};

