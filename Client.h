#pragma once

#include <SDL_net.h>

#include "World.h"

class Client
{
public:
	Client();
	~Client();

	void init(const char* ip, int port);
	void cleanup();

	void start();
	void stop();

	void send(char* data, int size);

	inline UDPsocket& getSocket() { return m_socket; }
	inline UDPpacket* getSPacket() { return m_spacket; }
	inline UDPpacket* getRPacket() { return m_rpacket; }
	inline bool isRunning() { return m_running; }

	inline World* getWorld() { return m_world; }
	inline void setWorld(World* world) { m_world = world; }
private:
	UDPsocket m_socket;
	UDPpacket* m_spacket;
	UDPpacket* m_rpacket;
	IPaddress m_serverAddress;
	SDL_Thread* m_thread;
	bool m_running;

	World* m_world;
};

extern int clientThread(void* parameters);