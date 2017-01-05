#include "RenderingEngine.h"

#include <glew.h>

bool RenderingEngine::m_wireframe;

void RenderingEngine::init()
{
	SDL_Init(SDL_INIT_EVERYTHING); // Initializes SDL2
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Enables double buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // Tells the depth buffer to use 24 bits per pixel

	setWireframe(false);
}

void RenderingEngine::cleanup()
{
	SDL_Quit(); // Quits SDL2
}

void RenderingEngine::setWireframe(bool wireframe)
{
	m_wireframe = wireframe;
	glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
}

void RenderingEngine::toggleWireframe()
{
	m_wireframe = !m_wireframe;
}