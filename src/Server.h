#pragma once

#include <SDL_net.h>

#include "World.h"

class Server
{
public:
	Server();
	~Server();

	void init();
	void cleanup();

	void start();
	void stop();

	void send(char* data, int size, IPaddress& address);
	void sendToAll(char* data, int size);

	inline UDPsocket& getSocket() { return m_socket; }
	inline UDPpacket* getSPacket() { return m_spacket; }
	inline UDPpacket* getRPacket() { return m_rpacket; }
	inline World* getWorld() { return m_world; }
	inline bool isRunning() { return m_running; }
private:
	UDPsocket m_socket;
	UDPpacket* m_spacket;
	UDPpacket* m_rpacket;
	SDL_Thread* m_thread;
	bool m_running;

	World* m_world;
};

extern int serverThread(void* parameters);