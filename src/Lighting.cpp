#include "Lighting.h"

#include <glew.h>

#define MAX_LIGHTS 128

Lighting::Lighting()
{
}

Lighting::~Lighting()
{
}

void Lighting::init(unsigned int width, unsigned int height)
{
	int attachments[1];
	attachments[0] = GL_COLOR_ATTACHMENT0;
	int textureTypes[1];
	textureTypes[0] = 1;
	m_renderTarget.init(1, width, height, attachments, textureTypes);
	m_shader.init("Resources/lighting.vs", "Resources/lighting.fs");
}

void Lighting::render(Texture& worldTarget, Vec3& cameraPosition, float renderDistance)
{
	glPushAttrib(GL_POLYGON_MODE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_renderTarget.bindRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shader.enable();
	worldTarget.bind(1, 0);
	worldTarget.bind(2, 1);
	m_shader.setUniform("pixelPositions", 0);
	m_shader.setUniform("pixelNormals", 1);

	int currentIndex = 0;
	for (unsigned int i = 0; i < m_lights.size(); i++) {
		if ((m_lights[i].getPosition() - cameraPosition).length() > renderDistance) continue;
		setUniform("lights[" + std::to_string(currentIndex) + "]", m_lights[i]);
		currentIndex++;
	}
	m_shader.setUniform("numLights", currentIndex);

	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();
	m_shader.disable();
	glPopAttrib();
}

void Lighting::setUniform(std::string& name, Light& light)
{
	m_shader.setUniform(name + ".position", light.getPosition());
	m_shader.setUniform(name + ".color", light.getColor());
}