#include "Vec2.h"

#include <math.h>

Vec2::Vec2() : m_x(0.0f), m_y(0.0f)
{
}

Vec2::Vec2(float x, float y) : m_x(x), m_y(y)
{
}

Vec2::Vec2(float v) : m_x(v), m_y(v)
{
}

Vec2::~Vec2()
{
}

float Vec2::dot(Vec2& v)
{
	return m_x * v.m_x + m_y * v.m_y;
}

void Vec2::normalize()
{
	float l = length();
	if (l == 0.0f) return;
	m_x /= l;
	m_y /= l;
}

Vec2 Vec2::normalized()
{
	Vec2 v(m_x, m_y);
	v.normalize();
	return v;
}

float Vec2::length()
{
	return sqrtf(m_x * m_x + m_y * m_y);
}

Vec2 Vec2::operator+(Vec2& v)
{
	return Vec2(m_x + v.m_x, m_y + v.m_y);
}

Vec2 Vec2::operator-(Vec2& v)
{
	return Vec2(m_x - v.m_x, m_y - v.m_y);
}

Vec2 Vec2::operator*(Vec2& v)
{
	return Vec2(m_x * v.m_x, m_y * v.m_y);
}

Vec2 Vec2::operator/(Vec2& v)
{
	return Vec2(m_x / v.m_x, m_y / v.m_y);
}

Vec2 Vec2::operator+(float v)
{
	return Vec2(m_x + v, m_y + v);
}

Vec2 Vec2::operator-(float v)
{
	return Vec2(m_x - v, m_y - v);
}

Vec2 Vec2::operator*(float v)
{
	return Vec2(m_x * v, m_y * v);
}

Vec2 Vec2::operator/(float v)
{
	return Vec2(m_x / v, m_y / v);
}

void Vec2::set(float x, float y)
{
	m_x = x;
	m_y = y;
}