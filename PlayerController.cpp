#include "PlayerController.h"

#include "Human.h"
#include "Input.h"

PlayerController::PlayerController()
{
}


PlayerController::~PlayerController()
{
}

void PlayerController::update(float delta)
{
	Entity* entity = getEntity();
	Human* human = (Human*)entity;

	float sensitivity = 0.2f;
	if (Input::isKeyDown(SDLK_ESCAPE)) Input::setMouseGrabbed(false);
	if (Input::isKeyDown(1)) Input::setMouseGrabbed(true);

	if (Input::isMouseGrabbed()) {
		human->setRotation(human->getRotation() + Vec3((float)Input::getDY(), (float)Input::getDX(), 0.0f) * sensitivity);
	}

	Vec3 direction(0.0f, 0.0f, 0.0f);
	if (Input::isKeyDown(SDLK_w)) direction = direction + Vec3(0.0f, 0.0f, -1.0f);
	if (Input::isKeyDown(SDLK_a)) direction = direction + Vec3(-1.0f, 0.0f, 0.0f);
	if (Input::isKeyDown(SDLK_s)) direction = direction + Vec3(0.0f, 0.0f, 1.0f);
	if (Input::isKeyDown(SDLK_d)) direction = direction + Vec3(1.0f, 0.0f, 0.0f);
	
	float speed = 0.02f;
	if (Input::isKeyDown(SDLK_LCTRL)) speed *= 8.0f;

	float sin = sinf(human->getRotation().getY() * (float)M_PI / 180.0f);
	float cos = cosf(human->getRotation().getY() * (float)M_PI / 180.0f);
	
	Vec3 velocity(direction.getX() * cos - direction.getZ() * sin, direction.getY(), direction.getZ() * cos + direction.getX() * sin);
	human->setVelocity(human->getVelocity() + velocity * speed);

	if (Input::isKeyDown(SDLK_SPACE)) {
		if (human->isOnGround()) {
			human->setVelocity(human->getVelocity() + Vec3(0.0f, 0.04f, 0.0f));
		}
	}
	human->updateData();
}