#pragma once

#include "EntityController.h"

class PlayerController : public EntityController
{
public:
	PlayerController();
	~PlayerController();

	void update(float delta);
	virtual inline EntityControllerType getType() { return EntityControllerType::CONTROLLER_PLAYER; }
};

