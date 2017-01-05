#pragma once

#include "Transform.h"

class Mat4;

class Camera
{
public:
	Camera();
	Camera(float x, float y, float z);
	~Camera();

	void debugUpdate(float delta);
	Mat4 getMatrix();

	inline Transform& getTransform() { return m_transform; }
	inline Vec3& getPosition() { return m_transform.getTranslation(); }
	inline Vec3& getRotation() { return m_transform.getRotation(); }
private:
	Transform m_transform;
};

