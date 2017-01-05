#include "Camera.h"

#include <glew.h>

#include "Input.h"
#include "Mat4.h"

Camera::Camera() : m_transform()
{
}

Camera::Camera(float x, float y, float z) : m_transform()
{
	m_transform.setTranslation(Vec3(x, y, z));
}

Camera::~Camera()
{
}

void Camera::debugUpdate(float delta)
{
	float sensitivity = 0.2f;
	if (Input::isKeyDown(SDLK_ESCAPE)) Input::setMouseGrabbed(false);
	if (Input::isKeyDown(1)) Input::setMouseGrabbed(true);

	if (Input::isMouseGrabbed()) {
		m_transform.setRotation(m_transform.getRotation() + Vec3((float)Input::getDY(), (float)Input::getDX(), 0.0f) * sensitivity);
	}

	Vec3 direction(0.0f, 0.0f, 0.0f);
	if (Input::isKeyDown(SDLK_w)) direction = direction + Vec3(0.0f, 0.0f, -1.0f);
	if (Input::isKeyDown(SDLK_a)) direction = direction + Vec3(-1.0f, 0.0f, 0.0f);
	if (Input::isKeyDown(SDLK_s)) direction = direction + Vec3(0.0f, 0.0f, 1.0f);
	if (Input::isKeyDown(SDLK_d)) direction = direction + Vec3(1.0f, 0.0f, 0.0f);
	if (Input::isKeyDown(SDLK_SPACE)) direction = direction + Vec3(0.0f, 1.0f, 0.0f);
	if (Input::isKeyDown(SDLK_LSHIFT)) direction = direction + Vec3(0.0f, -1.0f, 0.0f);

	float speed = delta * 0.02f;
	if (Input::isKeyDown(SDLK_LCTRL)) speed *= 8.0f;

	float sin = sinf(m_transform.getRotation().getY() * (float)M_PI / 180.0f);
	float cos = cosf(m_transform.getRotation().getY() * (float)M_PI / 180.0f);

	Vec3 velocity(direction.getX() * cos - direction.getZ() * sin, direction.getY(), direction.getZ() * cos + direction.getX() * sin);
	m_transform.setTranslation(m_transform.getTranslation() + velocity * speed);
}

Mat4 Camera::getMatrix()
{
	Mat4 m;
	m.camera(*this);
	return m;
}