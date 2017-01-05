#include "Vec3.h"

#include <math.h>

Vec3::Vec3() : m_x(0.0f), m_y(0.0f), m_z(0.0f)
{
}

Vec3::Vec3(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
{
}

Vec3::Vec3(float v) : m_x(v), m_y(v), m_z(v)
{
}

Vec3::~Vec3()
{
}

float Vec3::dot(Vec3& v)
{
	return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
}

Vec3 Vec3::cross(Vec3& v)
{
	return Vec3(m_y * v.m_z - m_z * v.m_y, m_z * v.m_x - m_x * v.m_z, m_x * v.m_y - m_y * v.m_x);
}

void Vec3::normalize()
{
	float l = length();
	if (l == 0.0f) return;
	m_x /= l;
	m_y /= l;
	m_z /= l;
}

Vec3 Vec3::normalized()
{
	Vec3 v(m_x, m_y, m_z);
	v.normalize();
	return v;
}

float Vec3::length()
{
	return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
}

Vec3 Vec3::operator+(Vec3& v)
{
	return Vec3(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
}

Vec3 Vec3::operator-(Vec3& v)
{
	return Vec3(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
}

Vec3 Vec3::operator*(Vec3& v)
{
	return Vec3(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z);
}

Vec3 Vec3::operator/(Vec3& v)
{
	return Vec3(m_x / v.m_x, m_y / v.m_y, m_z / v.m_z);
}

bool Vec3::operator==(const Vec3& v) const
{
	return m_x == v.m_x && m_y == v.m_y && m_z == v.m_z;
}

Vec3 Vec3::operator+(float v)
{
	return Vec3(m_x + v, m_y + v, m_z + v);
}

Vec3 Vec3::operator-(float v)
{
	return Vec3(m_x - v, m_y - v, m_z - v);
}

Vec3 Vec3::operator*(float v)
{
	return Vec3(m_x * v, m_y * v, m_z * v);
}

Vec3 Vec3::operator/(float v)
{
	return Vec3(m_x / v, m_y / v, m_z / v);
}

void Vec3::set(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}