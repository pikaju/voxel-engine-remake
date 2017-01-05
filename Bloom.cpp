#include "Bloom.h"

#include <glew.h>

Bloom::Bloom()
{
}

Bloom::~Bloom()
{
}

void Bloom::init(unsigned int width, unsigned int height)
{
	m_shader.init("Resources/bloom.vs", "Resources/bloom.fs");
	int attachments[1];
	attachments[0] = GL_COLOR_ATTACHMENT0;
	int textureTypes[1];
	textureTypes[0] = 0;
	m_renderTarget.init(1, width, height, attachments, textureTypes);
}

void Bloom::render(Texture& texture)
{
	m_renderTarget.bindRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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