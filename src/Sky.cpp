#include "Sky.h"

#include <glew.h>

Sky::Sky() : m_sunDirection(0.0f, 1.0f, 0.0f)
{
}

Sky::~Sky()
{
}

void Sky::init(unsigned int width, unsigned int height)
{
	int attachments[1];
	attachments[0] = GL_COLOR_ATTACHMENT0;
	int textureTypes[1];
	textureTypes[0] = 0;
	m_renderTarget.init(1, width, height, attachments, textureTypes);
	m_shader.init("Resources/sky.vs", "Resources/sky.fs");
}

void Sky::render(Camera& camera, Mat4& projectionMatrix)
{
	Mat4 modelMatrix;
	modelMatrix.translate(camera.getPosition().getX(), camera.getPosition().getY(), camera.getPosition().getZ());
	Mat4 mvp = modelMatrix * camera.getMatrix() * projectionMatrix;

	m_renderTarget.bindRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	m_shader.enable();
	m_shader.setUniform("mvpMatrix", mvp);
	m_shader.setUniform("sunDirection", m_sunDirection);
	glBegin(GL_QUADS);
	float size = 1024.0f;

	glVertex3f(-size, size, -size);
	glVertex3f(size, size, -size);
	glVertex3f(size, size, size);
	glVertex3f(-size, size, size);

	glVertex3f(-size, -size, -size);
	glVertex3f(-size, -size, size);
	glVertex3f(size, -size, size);
	glVertex3f(size, -size, -size);

	glVertex3f(-size, -size, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, -size, size);

	glVertex3f(size, -size, -size);
	glVertex3f(size, -size, size);
	glVertex3f(size, size, size);
	glVertex3f(size, size, -size);

	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, size, -size);
	glVertex3f(-size, size, -size);

	glVertex3f(-size, -size, size);
	glVertex3f(-size, size, size);
	glVertex3f(size, size, size);
	glVertex3f(size, -size, size);

	glEnd();
	m_shader.disable();
	glEnable(GL_DEPTH_TEST);
}