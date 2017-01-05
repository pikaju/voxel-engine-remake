#pragma once

#include <SDL_net.h>

#include "EntityController.h"

class ConnectionController : public EntityController
{
public:
	ConnectionController();
	~ConnectionController();

	inline IPaddress& getAddress() { return m_address; }
	inline void setAddress(IPaddress address) { m_address = address; }
	virtual inline EntityControllerType getType() { return EntityControllerType::CONTROLLER_CONNECTION; }
private:
	IPaddress m_address;
};

