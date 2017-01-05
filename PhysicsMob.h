#pragma once

#include "Mob.h"

class PhysicsMob : public Mob
{
public:
	PhysicsMob();
	~PhysicsMob();

	virtual void update(float delta);

	virtual void addData(DataContainer& container);
	virtual void getData(DataContainer& container);

	inline Vec3& getVelocity() { return m_velocity; }
	inline void setVelocity(Vec3& velocity) { m_velocity = velocity; }

	inline bool isOnGround() { return m_onGround; }
private:
	Vec3 m_velocity;
	bool m_onGround;
};

