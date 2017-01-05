#pragma once

#include "Vec3.h"
#include "Entity.h"

class Mob : public Entity
{
public:
	Mob();
	~Mob();

	virtual void addData(DataContainer& container);
	virtual void getData(DataContainer& container);

	bool move(Vec3& movement);
	bool intersectsTerrain();

	inline Vec3& getPosition() { return m_position; }
	inline void setPosition(Vec3& position) { m_position = position; }
	inline Vec3& getSize() { return m_size; }
	inline void setSize(Vec3& size) { m_size = size; }
private:
	Vec3 m_position;
	Vec3 m_size;
};

