#pragma once
class Vec2
{
public:
	Vec2();
	Vec2(float x, float y);
	Vec2(float v);
	~Vec2();

	float dot(Vec2& v);

	void normalize();
	Vec2 normalized();

	float length();

	Vec2 operator+(Vec2& v);
	Vec2 operator-(Vec2& v);
	Vec2 operator*(Vec2& v);
	Vec2 operator/(Vec2& v);

	Vec2 operator+(float v);
	Vec2 operator-(float v);
	Vec2 operator*(float v);
	Vec2 operator/(float v);

	void set(float x, float y);
	inline void set(float v) { set(v, v); }
	inline void set(Vec2& v) { set(v.m_x, v.m_y); }

	inline float getX() { return m_x; }
	inline float getY() { return m_y; }

	inline void setX(float x) { m_x = x; }
	inline void setY(float y) { m_y = y; }
private:
	float m_x;
	float m_y;
};

