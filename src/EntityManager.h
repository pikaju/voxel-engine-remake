#pragma once

#include <hash_map>

#include "DataContainer.h"

class World;
class Entity;
class PlayerController;
class ConnectionController;

class EntityManager
{
public:
	EntityManager(World* world);
	~EntityManager();

	void update(float delta);
	void render();

	void addEntity(unsigned int id, Entity* entity);
	void addEntity(Entity* entity);
	Entity* addEntity(DataContainer& container);
	void updateEntity(Entity* entity);
	void updateEntity(DataContainer& container);
	Entity* getEntity(unsigned int id);
	void removeEntity(unsigned int id);
	inline std::vector<Entity*>& getEntities() { return m_entitiesVector; }
	unsigned int getUnusedID();

	PlayerController* getLocalPlayer();
	void getConnections(std::vector<ConnectionController*>& connections);
private:
	World* m_world;

	std::hash_map<unsigned int, Entity*> m_entities;
	std::vector<Entity*> m_entitiesVector;
};

