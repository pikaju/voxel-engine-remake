#include "Mob.h"


Mob::Mob() : m_position(), m_size()
{
}

Mob::~Mob()
{
}

void Mob::addData(DataContainer& container)
{
	Entity::addData(container);
	container.addVec3(m_position);
	container.addVec3(m_size);
}

void Mob::getData(DataContainer& container)
{
	Entity::getData(container);
	m_position = container.getVec3();
	m_size = container.getVec3();
}

bool Mob::move(Vec3& movement)
{
	m_position = m_position + movement;
	if (intersectsTerrain()) {
		m_position = m_position - movement;
		return true;
	}
	return false;
}

bool Mob::intersectsTerrain()
{
	ChunkManager& c = getWorld()->getChunkManager();
	for (float x = 0.0f; x <= m_size.getX(); x += m_size.getX() - x < 1.0f ? 0.1f : 1.0f) {
		for (float y = 0.0f; y <= m_size.getY(); y += m_size.getY() - y < 1.0f ? 0.1f : 1.0f) {
			for (float z = 0.0f; z <= m_size.getZ(); z += m_size.getZ() - z < 1.0f ? 0.1f : 1.0f) {
				Vec3 position = m_position + Vec3(x, y, z);
				if (c.isSolid((int)floorf(position.getX()), (int)floorf(position.getY()), (int)floorf(position.getZ()))) {
					return true;
				}
			}
		}
	}
	return false;
}