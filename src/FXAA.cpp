#include "FXAA.h"

#include <glew.h>

FXAA::FXAA()
{
}

FXAA::~FXAA()
{
}

void FXAA::init()
{
	m_shader.init("Resources/fxaa.vs", "Resources/fxaa.fs");
}

void FXAA::render(Texture& texture)
{
	m_shader.enable();
	m_shader.setUniform("pixelColors", 0);
	m_shader.setUniform("texelSize", Vec2(1.0f / (float)texture.getWidth(), 1.0f / (float)texture.getHeight()));
	texture.bind(0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();
	m_shader.disable();
}