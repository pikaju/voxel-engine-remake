#include "Shadow.h"

#include <glew.h>
#include <SDL_timer.h>

#include "World.h"
#include "FrustumCuller.h"

Shadow::Shadow()
{
}

Shadow::~Shadow()
{
}

void Shadow::init(int shadowMapSize, float shadowMapDensity)
{
	m_shadowMapDensity = shadowMapDensity;
	int attachments[1];
	attachments[0] = GL_COLOR_ATTACHMENT0;
	int textureTypes[1];
	textureTypes[0] = 1;
	int filters[1];
	filters[0] = GL_NEAREST;
	m_renderTarget.init(1, shadowMapSize, shadowMapSize, attachments, textureTypes, (void*)0, filters);
	m_shader.init("Resources/shadow.vs", "Resources/shadow.fs");
}

void Shadow::render(World* world)
{	
	if (rand() % 1 != 0) return;
	m_renderTarget.bindRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shader.enable();
	Camera camera;
	camera.getTransform().setTranslation(world->getCamera().getPosition());
	camera.getTransform().setRotation(Vec3(90.0f + sinf(SDL_GetTicks() * 0.0001f) * 45.0f, 20.0f, 0.0f));
	Mat4 projectionMatrix;
	float projectionLength = 4096.0f;
	projectionMatrix.ortho(-m_shadowMapDensity, m_shadowMapDensity, -m_shadowMapDensity, m_shadowMapDensity, -projectionLength * 0.5f, projectionLength * 0.5f);
	m_mvpMatrix = camera.getMatrix() * projectionMatrix;
	m_shader.setUniform("modelMatrix", Mat4());
	m_shader.setUniform("viewMatrix", camera.getMatrix());
	m_shader.setUniform("projectionMatrix", projectionMatrix);
	FrustumCuller::extractPlanes(m_mvpMatrix);
	world->renderScene();
	m_shader.disable();
}