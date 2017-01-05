#include "Entity.h"

#include "EntityController.h"

#include <iostream>

Entity::Entity() : m_id(0), m_removed(false), m_type(EntityType::ENTITY_UNDEFINED), m_world(nullptr), m_controller(nullptr)
{
}

Entity::~Entity()
{
	if (m_controller != nullptr) {
		delete m_controller;
	}
}

void Entity::update(float delta)
{
	if (m_controller != nullptr) {
		m_controller->update(delta);
	}
}

void Entity::render()
{

}

void Entity::addData(DataContainer& container)
{
	container.addInt(m_id);
	container.addInt(getType());
	container.addBool(m_removed);
}

void Entity::getData(DataContainer& container)
{
	m_id = container.getInt();
	m_type = (EntityType)container.getInt();
	m_removed = container.getBool();
}

EntityControllerType Entity::getControllerType()
{
	if (m_controller == nullptr) return EntityControllerType::CONTROLLER_UNDEFINED;
	return m_controller->getType();
}