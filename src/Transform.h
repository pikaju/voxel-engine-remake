#pragma once

#include "Vec3.h"

class Transform
{
public:
	Transform(Vec3 translation = Vec3(), Vec3 rotation = Vec3(), Vec3 scale = Vec3(1.0f, 1.0f, 1.0f));
	~Transform();

	inline Vec3& getTranslation() { return m_translation; }
	inline Vec3& getRotation() { return m_rotation; }
	inline Vec3& getScale() { return m_scale; }

	inline void setTranslation(const Vec3& translation) { m_translation = translation; }
	inline void setRotation(const Vec3& rotation) { m_rotation = rotation; }
	inline void setScale(const Vec3& scale) { m_scale = scale; }
private:
	Vec3 m_translation;
	Vec3 m_rotation;
	Vec3 m_scale;
};

