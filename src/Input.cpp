#include "Input.h"

std::unordered_map<long, bool> Input::m_keys;
std::unordered_map<long, bool> Input::m_recentEvents;
int Input::m_x;
int Input::m_y;
int Input::m_dx;
int Input::m_dy;

void Input::processEvent(SDL_Event& windowEvent)
{
	if (windowEvent.type == SDL_KEYDOWN) {
		m_keys[windowEvent.key.keysym.sym] = true;
		m_recentEvents[windowEvent.key.keysym.sym] = true;
	}
	if (windowEvent.type == SDL_KEYUP) m_keys[windowEvent.key.keysym.sym] = false;
	if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
		m_keys[windowEvent.button.button] = true;
		m_recentEvents[windowEvent.button.button] = true;
	}
	if (windowEvent.type == SDL_MOUSEBUTTONUP) m_keys[windowEvent.button.button] = false;
	if (windowEvent.type == SDL_MOUSEMOTION) {
		m_x = windowEvent.motion.x;
		m_y = windowEvent.motion.y;
		m_dx += windowEvent.motion.xrel;
		m_dy += windowEvent.motion.yrel;
	}
}

void Input::refresh()
{
	m_dx = 0;
	m_dy = 0;
	m_recentEvents.clear();
}

bool Input::isKeyDown(long key)
{
	if (m_keys.find(key) == m_keys.end()) return false;
	return m_keys[key];
}

bool Input::getEvent(long key, bool state)
{
	if (m_keys.find(key) == m_keys.end()) return false;
	return m_recentEvents[key] == state;
}

void Input::setMouseGrabbed(bool mouseGrabbed)
{
	SDL_SetRelativeMouseMode(mouseGrabbed ? SDL_TRUE : SDL_FALSE);
}

bool Input::isMouseGrabbed()
{
	return SDL_GetRelativeMouseMode() == SDL_TRUE;
}