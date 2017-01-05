#include "PhysicsMob.h"

#define GRAVITY -0.08f

PhysicsMob::PhysicsMob() : Mob(), m_velocity(), m_onGround(false)
{
}

PhysicsMob::~PhysicsMob()
{
}

void PhysicsMob::update(float delta)
{
	Mob::update(delta);
	m_velocity.setY(m_velocity.getY() + GRAVITY * (delta / 1000.0f));
	m_onGround = false;
	if (move(Vec3(0.0f, m_velocity.getY(), 0.0f) * delta)) {
		if (m_velocity.getY() < 0.0f) {
			m_onGround = true;
		}
		m_velocity.setY(0.0f);
	}
	move(Vec3(m_velocity.getX(), 0.0f, 0.0f) * delta);
	move(Vec3(0.0f, 0.0f, m_velocity.getZ()) * delta);
	m_velocity.setX(0.0f);
	m_velocity.setZ(0.0f);
}

void PhysicsMob::addData(DataContainer& container)
{
	Mob::addData(container);
	container.addVec3(m_velocity);
}

void PhysicsMob::getData(DataContainer& container)
{
	Mob::getData(container);
	m_velocity = container.getVec3();
}