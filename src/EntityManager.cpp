#include "EntityManager.h"

#include "World.h"
#include "Server.h"
#include "Client.h"
#include "Entity.h"
#include "Human.h"
#include "PlayerController.h"
#include "ConnectionController.h"

EntityManager::EntityManager(World* world) : m_world(world)
{
}

EntityManager::~EntityManager()
{
	for (unsigned int i = 0; i < m_entitiesVector.size(); i++) {
		delete m_entitiesVector[i];
	}
}

void EntityManager::update(float delta)
{
	for (unsigned int i = 0; i < m_entitiesVector.size(); i++) {
		Entity* e = m_entitiesVector[i];
		e->update(delta);
		if (e->isRemoved()) {
			removeEntity(e->getID());
			i--;
		}
	}
}

void EntityManager::render()
{
	for (unsigned int i = 0; i < m_entitiesVector.size(); i++) {
		Entity* e = m_entitiesVector[i];
		e->render();
	}
}

void EntityManager::addEntity(unsigned int id, Entity* entity)
{
	if (getEntity(id) != nullptr) removeEntity(id);
	entity->setID(id);
	entity->setWorld(m_world);
	m_entities[id] = entity;
	m_entitiesVector.push_back(entity);
	if (m_world->isServer()) {
		DataContainer c;
		c.addInt(0);
		entity->addData(c);
		m_world->getServer()->sendToAll(c.getData(), c.getSize());
	}
}

void EntityManager::addEntity(Entity* entity)
{
	if (m_world->isServer()) {
		addEntity(getUnusedID(), entity);
	} else {
		DataContainer c;
		c.addInt(0);
		entity->addData(c);
		m_world->getClient()->send(c.getData(), c.getSize());
		delete entity;
	}
}

Entity* EntityManager::addEntity(DataContainer& container)
{
	int index = container.getIndex();
	Entity e;
	e.getData(container);
	container.setIndex(index);

	Entity* entity = nullptr;
	switch (e.getType()) {
	case (EntityType::ENTITY_HUMAN) : entity = new Human(); break;
	}
	entity->getData(container);
	
	if (m_world->isServer()) addEntity(entity);
	else addEntity(entity->getID(), entity);

	return entity;
}

void EntityManager::updateEntity(Entity* entity)
{
	DataContainer c;
	c.addInt(1);
	entity->addData(c);
	if (m_world->isServer()) {
		m_world->getServer()->sendToAll(c.getData(), c.getSize());
	} else {
		m_world->getClient()->send(c.getData(), c.getSize());
	}
}

void EntityManager::updateEntity(DataContainer& container)
{
	int index = container.getIndex();
	Entity e;
	e.getData(container);
	container.setIndex(index);

	Entity* entity = getEntity(e.getID());
	if (entity->getControllerType() != EntityControllerType::CONTROLLER_PLAYER) entity->getData(container);
	if (m_world->isServer()) {
		DataContainer c;
		c.addInt(1);
		entity->addData(c);
		m_world->getServer()->sendToAll(c.getData(), c.getSize());
	}
}

Entity* EntityManager::getEntity(unsigned int id)
{
	auto it = m_entities.find(id);
	if (it == m_entities.end()) return nullptr;
	return it->second;
}

void EntityManager::removeEntity(unsigned int id)
{
	auto it = m_entities.find(id);
	delete it->second;
	m_entities.erase(it);
	for (unsigned int i = 0; i < m_entitiesVector.size(); i++) {
		if (m_entitiesVector[i]->getID() == id) {
			m_entitiesVector[i] = m_entitiesVector.back();
			m_entitiesVector.pop_back();
		}
	}
}

unsigned int EntityManager::getUnusedID()
{
	unsigned int id = 0;
	while (m_entities.find(id) != m_entities.end()) id++;
	return id;
}

PlayerController* EntityManager::getLocalPlayer()
{
	for (unsigned int i = 0; i < m_entitiesVector.size(); i++) {
		EntityControllerType controllerType = m_entitiesVector[i]->getControllerType();
		if (controllerType == EntityControllerType::CONTROLLER_PLAYER) {
			return (PlayerController*)m_entitiesVector[i]->getController();
		}
	}
	return nullptr;
}

void EntityManager::getConnections(std::vector<ConnectionController*>& connections)
{
	for (unsigned int i = 0; i < m_entitiesVector.size(); i++) {
		EntityControllerType controllerType = m_entitiesVector[i]->getControllerType();
		if (controllerType == EntityControllerType::CONTROLLER_CONNECTION) {
			connections.push_back((ConnectionController*)m_entitiesVector[i]->getController());
		}
	}
}