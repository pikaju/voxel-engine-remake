#include "Vec3i.h"

#include <math.h>

Vec3i::Vec3i() : m_x(0), m_y(0), m_z(0)
{
}

Vec3i::Vec3i(int x, int y, int z) : m_x(x), m_y(y), m_z(z)
{
}

Vec3i::Vec3i(int v) : m_x(v), m_y(v), m_z(v)
{
}

Vec3i::Vec3i(Vec3& v) : m_x((int)floorf(v.getX())), m_y((int)floorf(v.getY())), m_z((int)floorf(v.getZ()))
{
}

Vec3i::~Vec3i()
{
}

int Vec3i::dot(Vec3i& v)
{
	return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
}

Vec3i Vec3i::cross(Vec3i& v)
{
	return Vec3i(m_y * v.m_z - m_z * v.m_y, m_z * v.m_x - m_x * v.m_z, m_x * v.m_y - m_y * v.m_x);
}

float Vec3i::length()
{
	return sqrtf((float)(m_x * m_x + m_y * m_y + m_z * m_z));
}

Vec3i Vec3i::operator+(Vec3i& v)
{
	return Vec3i(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
}

Vec3i Vec3i::operator-(Vec3i& v)
{
	return Vec3i(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
}

Vec3i Vec3i::operator*(Vec3i& v)
{
	return Vec3i(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z);
}

Vec3i Vec3i::operator/(Vec3i& v)
{
	return Vec3i(m_x / v.m_x, m_y / v.m_y, m_z / v.m_z);
}

bool Vec3i::operator==(const Vec3i& v) const
{
	return m_x == v.m_x && m_y == v.m_y && m_z == v.m_z;
}

Vec3i Vec3i::operator+(float v)
{
	return Vec3i((int)(m_x + v), (int)(m_y + v), (int)(m_z + v));
}

Vec3i Vec3i::operator-(float v)
{
	return Vec3i((int)(m_x - v), (int)(m_y - v), (int)(m_z - v));
}

Vec3i Vec3i::operator*(float v)
{
	return Vec3i((int)(m_x * v), (int)(m_y * v), (int)(m_z * v));
}

Vec3i Vec3i::operator/(float v)
{
	return Vec3i((int)(m_x / v), (int)(m_y / v), (int)(m_z / v));
}

void Vec3i::set(int x, int y, int z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}