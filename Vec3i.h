#pragma once

#include "Vec2.h"
#include "Vec3.h"

#include <xhash>

class Vec3i
{
public:
	Vec3i();
	Vec3i(int x, int y, int z);
	Vec3i(int v);
	Vec3i(Vec3& v);
	~Vec3i();

	int dot(Vec3i& v);
	Vec3i cross(Vec3i& v);

	float length();

	Vec3i operator+(Vec3i& v);
	Vec3i operator-(Vec3i& v);
	Vec3i operator*(Vec3i& v);
	Vec3i operator/(Vec3i& v);

	bool operator==(const Vec3i& v) const;

	Vec3i operator+(float v);
	Vec3i operator-(float v);
	Vec3i operator*(float v);
	Vec3i operator/(float v);

	void set(int x, int y, int z);
	inline void set(int v) { set(v, v, v); }
	inline void set(Vec3i& v) { set(v.getX(), v.getY(), v.getZ()); }

	inline int getX() const { return m_x; }
	inline int getY() const { return m_y; }
	inline int getZ() const { return m_z; }

	inline void setX(int x) { m_x = x; }
	inline void setY(int y) { m_y = y; }
	inline void setZ(int z) { m_z = z; }
private:
	int m_x;
	int m_y;
	int m_z;
};

template <>
struct std::hash<Vec3i>
{
	size_t operator()(const Vec3i& k) const {
		std::hash<int> h;
		return ((h(k.getX()) ^ (h(k.getY()) << 1)) >> 1) ^ (h(k.getZ()) << 1);
	}
};