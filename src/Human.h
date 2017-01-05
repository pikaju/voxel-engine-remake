#pragma once

#include "PhysicsMob.h"

class Human : public PhysicsMob
{
public:
	Human();
	~Human();

	virtual void render();

	virtual void addData(DataContainer& container);
	virtual void getData(DataContainer& container);

	inline Vec3& getRotation() { return m_rotation; }
	inline void setRotation(Vec3& rotation) { m_rotation = rotation; }

	inline Vec3& getOrigin() { return m_origin; }
private:
	Vec3 m_rotation;
	Vec3 m_origin;

	Model* m_modelHead;
	Model* m_modelBody;
	Model* m_modelFoot;
	Model* m_modelHand;
};

