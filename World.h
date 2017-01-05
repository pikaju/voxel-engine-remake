#pragma once

#include "ChunkManager.h"
#include "EntityManager.h"
#include "Camera.h"
#include "Noise.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
#include "ThreadManager.h"
#include "SSAO.h"
#include "FXAA.h"
#include "Bloom.h"
#include "Sky.h"
#include "Shadow.h"
#include "Lighting.h"

class Server;
class Client;

class PlayerController;

class World
{
public:
	World(Server* server, Client* client);
	~World();

	void cleanup();

	void update(float delta);
	void render(Window* window);
	void renderScene();

	inline ChunkManager& getChunkManager() { return m_chunkManager; }
	inline EntityManager& getEntityManager() { return m_entityManager; }
	inline Camera& getCamera() { return m_camera; }
	inline Noise& getNoise() { return m_noise; }
	inline int getVision() { return 16; }
	inline void setMaxFogDistance(float maxFogDistance) { m_maxFogDistance = maxFogDistance; }

	inline bool isServer() { return m_server != nullptr; }
	inline Server* getServer() { return m_server; }
	inline Client* getClient() { return m_client; }
private:
	Server* m_server;
	Client* m_client;

	Noise m_noise;
	ChunkManager m_chunkManager;
	EntityManager m_entityManager;
	Camera m_camera;
	Shader m_shader;
	Shader m_postProcessingShader;
	SSAO m_ssao;
	FXAA m_fxaa;
	Bloom m_bloom;
	Sky m_sky;
	Shadow m_shadow;
	Lighting m_lighting;

	Texture m_worldTarget;
	Texture m_postProcessingTarget;

	float m_fogDistance;
	float m_maxFogDistance;
};