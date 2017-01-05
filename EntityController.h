#pragma once

#include "Entity.h"

class EntityController
{
public:
	EntityController();
	~EntityController();

	virtual void update(float delta);

	inline void setEntity(Entity* entity) { m_entity = entity; }
	inline Entity* getEntity() { return m_entity; }
	virtual inline EntityControllerType getType() { return EntityControllerType::CONTROLLER_UNDEFINED; }
private:
	Entity* m_entity;
};

