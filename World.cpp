#include "World.h"

#include <string>
#include <iostream>
#include <glew.h>

#include "RenderingEngine.h"
#include "FrustumCuller.h"
#include "Server.h"
#include "PlayerController.h"
#include "Human.h"
#include "Profiler.h"
#include "Input.h"

World::World(Server* server, Client* client) : m_server(server), m_client(client), m_chunkManager(this), m_entityManager(this), m_camera(), m_noise(0), m_shader(), m_postProcessingShader(), m_sky(), m_ssao(), m_fxaa(), m_bloom(), m_lighting()
{
	if (isServer()) {
	} else {
		m_shader.init("Resources/world.vs", "Resources/world.fs");
		m_postProcessingShader.init("Resources/postProcessing.vs", "Resources/postProcessing.fs");
		int attachments[3];
		attachments[0] = GL_COLOR_ATTACHMENT0;
		attachments[1] = GL_COLOR_ATTACHMENT1;
		attachments[2] = GL_COLOR_ATTACHMENT2;
		int textureTypes[3];
		textureTypes[0] = 0;
		textureTypes[1] = 1;
		textureTypes[2] = 1;
		const int width = 1920;
		const int height = 1017;
		m_worldTarget.init(3, width, height, attachments, textureTypes);
		m_postProcessingTarget.init(1, width, height, attachments, textureTypes);
		m_ssao.init(width, height);
		m_fxaa.init();
		m_sky.init(width, height);
		m_bloom.init(width, height);
		m_lighting.init(width, height);
		float currentDensity = (float)(getVision() * CHUNK_SIZE);
		m_shadow.init(2048, currentDensity);
		//Human* human = new Human();
		//human->setPosition(Vec3(0.0f, 256.0f, 0.0f));
		//m_entityManager.addEntity(human);
	}
}

World::~World()
{
}

void World::cleanup()
{
}

void World::update(float delta)
{
	if (!isServer()) {
		m_fogDistance += (m_maxFogDistance - m_fogDistance) * delta * 0.002f;
		m_chunkManager.update(delta);
		m_entityManager.update(delta);
		PlayerController* player = m_entityManager.getLocalPlayer();
		if (player != nullptr) {
			m_camera.getTransform().setTranslation(((Human*)player->getEntity())->getPosition() + ((Human*)player->getEntity())->getOrigin());
			m_camera.getTransform().setRotation(((Human*)player->getEntity())->getRotation());
		} else {
			m_camera.debugUpdate(delta);
		}
		if (Input::getEvent(SDLK_2, true)) {
			m_lighting.addLight(Light(m_camera.getPosition(), Vec3(rand() % 65536 / 65536.0f, rand() % 65536 / 65536.0f, rand() % 65536 / 65536.0f).normalized()));
		}
		if (Input::getEvent(SDL_BUTTON_LEFT, true)) {
			Vec3i position;
			if (m_chunkManager.pick(m_camera.getPosition(), m_camera.getRotation(), position)) {
				m_chunkManager.setBlock(position.getX(), position.getY(), position.getZ(), 0x00000000);
			}
		}
	} else {
		m_entityManager.update(delta);
	}
}

void World::render(Window* window)
{
	Profiler::start("Rendering scene");
	m_worldTarget.bindRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderingEngine::setWireframe();
	Mat4 projection;
	projection.projection(90.0f, window->getAspectRatio(), 0.25f, 8192.0f);
	m_shader.enable();
	m_shader.setUniform("modelMatrix", Mat4());
	m_shader.setUniform("viewMatrix", m_camera.getMatrix());
	m_shader.setUniform("projectionMatrix", projection);
	FrustumCuller::extractPlanes(m_camera.getMatrix() * projection);
	renderScene();
	Profiler::stop();

	Profiler::start("Rendering shadows");
	//m_shadow.render(this);
	Profiler::stop();

	Profiler::start("Rendering lighting");
	m_lighting.render(m_worldTarget, m_camera.getPosition(), (float)(getVision() * CHUNK_SIZE) * 0.5f);
	Profiler::stop();

	Profiler::start("Rendering SSAO");
	m_ssao.render(m_worldTarget, m_camera.getPosition());
	Profiler::stop();

	Profiler::start("Rendering sky");
	m_sky.render(m_camera, projection);
	Profiler::stop();

	Profiler::start("Rendering post processing");
	m_postProcessingTarget.bindRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_postProcessingShader.enable();
	m_postProcessingShader.setUniform("pixelColors", 0);
	m_postProcessingShader.setUniform("pixelPositions", 1);
	m_postProcessingShader.setUniform("pixelLighting", 2);
	m_postProcessingShader.setUniform("pixelSSAO", 3);
	m_postProcessingShader.setUniform("pixelSky", 4);
	m_postProcessingShader.setUniform("fogDistance", m_fogDistance);
	m_postProcessingShader.setUniform("cameraPosition", m_camera.getPosition());
	m_postProcessingShader.setUniform("pixelShadow", 4);
	m_postProcessingShader.setUniform("shadowMapMVP", m_shadow.getMVPMatrix());
	m_postProcessingShader.setUniform("shadowMapTexelSize", Vec2(1.0f / m_shadow.getRenderTarget().getWidth(), 1.0f / m_shadow.getRenderTarget().getHeight()));
	m_shadow.getRenderTarget().bind(0, 4);
	m_worldTarget.bind(0, 0);
	m_worldTarget.bind(1, 1);
	m_lighting.getRenderTarget().bind(0, 2);
	m_ssao.getRenderTarget().bind(0, 3);
	m_sky.getRenderTarget().bind(0, 4);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();
	m_postProcessingShader.disable();
	Profiler::stop();

	Profiler::start("Rendering bloom");
	m_bloom.render(m_postProcessingTarget);
	Profiler::stop();

	Profiler::start("Rendering FXAA");
	window->bindRenderTarget();
	m_fxaa.render(m_bloom.getRenderTarget());
	Profiler::stop();
}

void World::renderScene()
{
	m_chunkManager.render();
	m_entityManager.render();
}