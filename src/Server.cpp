#include "Server.h"

#include <iostream>

#include "ConnectionController.h"

#include "DataContainer.h"

Server::Server() : m_running(false)
{
}

Server::~Server()
{
}

void Server::init()
{
	if (!(m_socket = SDLNet_UDP_Open(25565))) {
		std::cout << "Failed to create server socket" << std::endl;
	}
	if (!(m_spacket = SDLNet_AllocPacket(1024))) {
		std::cout << "Failed to allocate server packet" << std::endl;
	}
	if (!(m_rpacket = SDLNet_AllocPacket(1024))) {
		std::cout << "Failed to allocate server packet" << std::endl;
	}
	m_world = new World(this, nullptr);
}

void Server::cleanup()
{
	m_spacket->data = 0;
	SDLNet_FreePacket(m_spacket);
	m_rpacket->data = 0;
	SDLNet_FreePacket(m_rpacket);
}

void Server::start()
{
	if (m_running) return;
	m_running = true;
	m_thread = SDL_CreateThread(serverThread, "Server", (void*)this);
}

void Server::stop()
{
	if (!m_running) return;
	m_running = false;
	int status;
	SDL_WaitThread(m_thread, &status);
}

void Server::send(char* data, int size, IPaddress& address)
{
	m_spacket->address.host = address.host;
	m_spacket->address.port = address.port;
	m_spacket->data = (Uint8*)data;
	m_spacket->len = size;
	SDLNet_UDP_Send(m_socket, -1, m_spacket);
}

void Server::sendToAll(char* data, int size)
{
	std::vector<ConnectionController*> connections;
	m_world->getEntityManager().getConnections(connections);
	for (unsigned int i = 0; i < connections.size(); i++) {
		send(data, size, connections[i]->getAddress());
	}
}

int serverThread(void* parameters)
{
	Server* server = (Server*)parameters;
	while (server->isRunning()) {
		if (SDLNet_UDP_Recv(server->getSocket(), server->getRPacket())) {
			DataContainer c;
			c.addData((char*)server->getRPacket()->data, server->getRPacket()->len);
			int packetType = c.getInt();
			if (packetType == 0) {
				Entity* entity = server->getWorld()->getEntityManager().addEntity(c);
				if (entity->getType() == EntityType::ENTITY_HUMAN) {
					ConnectionController* controller = new ConnectionController();
					entity->setController(controller);
					controller->setEntity(entity);
					controller->setAddress(server->getRPacket()->address);

					std::vector<Entity*>& entities = server->getWorld()->getEntityManager().getEntities();
					for (unsigned int i = 0; i < entities.size(); i++) {
						DataContainer c;
						c.addInt(0);
						entities[i]->addData(c);
						server->send(c.getData(), c.getSize(), server->getRPacket()->address);
					}

					DataContainer playerID;
					playerID.addInt(2);
					playerID.addInt(entity->getID());
					server->send(playerID.getData(), playerID.getSize(), server->getRPacket()->address);
				}
			}
			if (packetType == 1) {
				server->getWorld()->getEntityManager().updateEntity(c);
			}
		}
		SDL_Delay(1);
	}
	return 0;
}