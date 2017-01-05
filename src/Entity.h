#pragma once

#include "World.h"
#include "DataContainer.h"

class EntityController;

enum EntityType
{
	ENTITY_UNDEFINED, ENTITY_HUMAN
};

enum EntityControllerType
{
	CONTROLLER_UNDEFINED, CONTROLLER_PLAYER, CONTROLLER_CONNECTION
};

class Entity
{
public:
	Entity();
	~Entity();

	virtual void update(float delta);
	virtual void render();

	inline void updateData() { m_world->getEntityManager().updateEntity(this); }

	virtual void addData(DataContainer& container);
	virtual void getData(DataContainer& container);

	inline EntityType getType() { return m_type; }
	inline void setType(EntityType type) { m_type = type; }

	inline unsigned int getID() { return m_id; }
	inline void setID(unsigned int id) { m_id = id; }

	inline bool isRemoved() { return m_removed; }
	inline void remove() { m_removed = true; }
	
	inline World* getWorld() { return m_world; }
	inline void setWorld(World* world) { m_world = world; }

	inline EntityController* getController() { return m_controller; }
	inline void setController(EntityController* controller) { m_controller = controller; }
	EntityControllerType getControllerType();
private:
	unsigned int m_id;
	bool m_removed;
	EntityType m_type;
	World* m_world;
	EntityController* m_controller;
};

