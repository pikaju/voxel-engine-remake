#include "SSAO.h"

#include <glew.h>

SSAO::SSAO(unsigned int numSamples) : m_sampleKernel(new Vec2[numSamples]), m_numSamples(numSamples)
{
}

SSAO::~SSAO()
{
	delete[] m_sampleKernel;
}

void SSAO::init(unsigned int width, unsigned int height)
{
	int attachments[1];
	attachments[0] = GL_COLOR_ATTACHMENT0;
	int textureTypes[1];
	textureTypes[0] = 1;
	m_renderTarget.init(1, width, height, attachments, textureTypes);
	
	m_shader.init("Resources/ssao.vs", "Resources/ssao.fs");
	int index = 0;
	float distancePerSample = 2.0f / (sqrtf((float)m_numSamples) - 1.0f);
	for (float x = -1.0f; x <= 1.0f; x += distancePerSample) {
		for (float y = -1.0f; y <= 1.0f; y += distancePerSample) {
			m_sampleKernel[index] = Vec2(x, y).normalized() * (rand() % 65536 / 65536.0f * (rand() % 65536 / 65536.0f));
			index++;
		}
	}
}

void SSAO::render(Texture& worldTarget, Vec3& cameraPosition)
{
	glPushAttrib(GL_POLYGON_MODE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_renderTarget.bindRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shader.enable();
	m_shader.setUniform("pixelPositions", 0);
	m_shader.setUniform("cameraPosition", cameraPosition);
	m_shader.setUniform("sampleKernel", m_sampleKernel, m_numSamples);
	m_shader.setUniform("texelSize", Vec2(1.0f / (float)m_renderTarget.getWidth(), 1.0f / (float)m_renderTarget.getHeight()));
	worldTarget.bind(1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();
	m_shader.disable();
	glPopAttrib();
}