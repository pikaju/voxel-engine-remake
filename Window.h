#pragma once

#include <SDL.h>

class Window
{
public:
	Window();
	~Window();

	void init(const char* title, unsigned int width = 1280, unsigned int height = 720);
	void cleanup();

	void swapBuffers();

	int getWidth();
	int getHeight();
	float getAspectRatio();

	void setVisible(bool visible);
	void bindRenderTarget();
private:
	SDL_Window* m_window;
	SDL_GLContext m_context;
	bool m_visible;
};

