#include "Client.h"

#include <iostream>

#include "DataContainer.h"
#include "Entity.h"
#include "PlayerController.h"

Client::Client() : m_world(nullptr)
{
}

Client::~Client()
{
}

void Client::init(const char* ip, int port)
{
	if (!(m_socket = SDLNet_UDP_Open(0))) {
		std::cout << "Failed to open client socket" << std::endl;
	}
	if (!(m_spacket = SDLNet_AllocPacket(1024))) {
		std::cout << "Failed to open client socket" << std::endl;
	}
	if (!(m_rpacket = SDLNet_AllocPacket(1024))) {
		std::cout << "Failed to open client socket" << std::endl;
	}
	if (SDLNet_ResolveHost(&m_serverAddress, ip, port) == -1) {
		std::cout << "Failed to resolve host" << std::endl;
	}
}

void Client::cleanup()
{
	m_spacket->data = 0;
	SDLNet_FreePacket(m_spacket);
	m_rpacket->data = 0;
	SDLNet_FreePacket(m_rpacket);
}

void Client::start()
{
	if (m_running) return;
	m_running = true;
	m_thread = SDL_CreateThread(clientThread, "Client", (void*)this);
}

void Client::stop()
{
	if (!m_running) return;
	m_running = false;
	int status;
	SDL_WaitThread(m_thread, &status);
}

void Client::send(char* data, int size)
{
	m_spacket->address.host = m_serverAddress.host;
	m_spacket->address.port = m_serverAddress.port;
	m_spacket->data = (Uint8*)data;
	m_spacket->len = size;
	SDLNet_UDP_Send(m_socket, -1, m_spacket);
}

extern int clientThread(void* parameters)
{
	Client* client = (Client*)parameters;
	while (client->isRunning()) {
		if (SDLNet_UDP_Recv(client->getSocket(), client->getRPacket())) {
			DataContainer c;
			c.addData((char*)client->getRPacket()->data, client->getRPacket()->len);
			int packetType = c.getInt();
			if (packetType == 0) {
				client->getWorld()->getEntityManager().addEntity(c);
			}
			if (packetType == 1) {
				client->getWorld()->getEntityManager().updateEntity(c);
			}
			if (packetType == 2) {
				Entity* entity = client->getWorld()->getEntityManager().getEntity(c.getInt());
				PlayerController* controller = new PlayerController();
				entity->setController(controller);
				controller->setEntity(entity);
			}
		}
		SDL_Delay(1);
	}
	return 0;
}