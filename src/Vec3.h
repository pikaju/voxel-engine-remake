#pragma once

#include "Vec2.h"

#include <xhash>

class Vec3
{
public:
	Vec3();
	Vec3(float x, float y, float z);
	Vec3(float v);
	~Vec3();

	float dot(Vec3& v);
	Vec3 cross(Vec3& v);

	void normalize();
	Vec3 normalized();

	float length();

	Vec3 operator+(Vec3& v);
	Vec3 operator-(Vec3& v);
	Vec3 operator*(Vec3& v);
	Vec3 operator/(Vec3& v);

	bool operator==(const Vec3& v) const;

	Vec3 operator+(float v);
	Vec3 operator-(float v);
	Vec3 operator*(float v);
	Vec3 operator/(float v);

	void set(float x, float y, float z);
	inline void set(float v) { set(v, v, v); }
	inline void set(Vec3& v) { set(v.getX(), v.getY(), v.getZ()); }

	inline float getX() const { return m_x; }
	inline float getY() const { return m_y; }
	inline float getZ() const { return m_z; }

	inline void setX(float x) { m_x = x; }
	inline void setY(float y) { m_y = y; }
	inline void setZ(float z) { m_z = z; }

	inline Vec2 getXY() { return Vec2(m_x, m_y); }
	inline Vec2 getXZ() { return Vec2(m_x, m_z); }
	inline Vec2 getYZ() { return Vec2(m_y, m_z); }
private:
	float m_x;
	float m_y;
	float m_z;
};

template <>
struct std::hash<Vec3>
{
	size_t operator()(const Vec3& k) const {
		std::hash<float> h;
		return ((h(k.getX()) ^ (h(k.getY()) << 1)) >> 1) ^ (h(k.getZ()) << 1);
	}
};