#include "Window.h"

#include <glew.h>

Window::Window() : m_window(nullptr), m_context(0), m_visible(false)
{
}

Window::~Window()
{
	cleanup();
}

void Window::init(const char* title, unsigned int width, unsigned int height)
{
	// Creates the window
	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	
	if (m_window == 0) {
		m_window = nullptr;
		exit(1);
	}

	// Creates an OpenGL context
	m_context = SDL_GL_CreateContext(m_window);

	// Initializes GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		exit(2);
	}

	// Enables VSync
	SDL_GL_SetSwapInterval(0);
}

void Window::cleanup()
{
	// Deletes the OpenGL context if it exists
	if (m_context) {
		SDL_GL_DeleteContext(m_context);
		m_context = 0;
	}

	// Destroys the window if it exists
	if (m_window) {
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
}

void Window::swapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

int Window::getWidth()
{
	int w;
	int h;
	SDL_GetWindowSize(m_window, &w, &h);
	return w;
}

int Window::getHeight()
{
	int w;
	int h;
	SDL_GetWindowSize(m_window, &w, &h);
	return h;
}

float Window::getAspectRatio()
{
	int w;
	int h;
	SDL_GetWindowSize(m_window, &w, &h);
	return (float)w / (float)h;
}

void Window::setVisible(bool visible)
{
	if (m_visible == visible) return;
	m_visible = visible;
	if (m_visible) SDL_ShowWindow(m_window);
	else SDL_HideWindow(m_window);
}

void Window::bindRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	int width;
	int height;
	SDL_GetWindowSize(m_window, &width, &height);
	glViewport(0, 0, width, height);
}