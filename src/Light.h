#pragma once

#include "Vec3.h"

class Light
{
public:
	Light(Vec3& position, Vec3& color);
	~Light();

	inline Vec3& getPosition() { return m_position; }
	inline void setPosition(Vec3& position) { m_position = position; }

	inline Vec3& getColor() { return m_color; }
	inline void setColor(Vec3& color) { m_color = color; }
private:
	Vec3 m_position;
	Vec3 m_color;
};

