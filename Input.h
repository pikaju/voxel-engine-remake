#pragma once

#include <unordered_map>
#include <SDL_events.h>

class Input
{
public:
	static void processEvent(SDL_Event& windowEvent);
	static void refresh();
	static bool isKeyDown(long key);
	static bool getEvent(long key, bool state);
	static void setMouseGrabbed(bool mouseGrabbed);
	static bool isMouseGrabbed();

	static inline int getX() { return m_x; }
	static inline int getY() { return m_y; }
	static inline int getDX() { return m_dx; }
	static inline int getDY() { return m_dy; }
private:
	static std::unordered_map<long, bool> m_keys;
	static std::unordered_map<long, bool> m_recentEvents;
	static int m_x;
	static int m_y;
	static int m_dx;
	static int m_dy;
};

