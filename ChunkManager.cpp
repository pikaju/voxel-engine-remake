#include "ChunkManager.h"

#include <string>
#include <algorithm>
#include <SDL_timer.h>
#include <iostream>

#include "World.h"
#include "ThreadManager.h"
#include "Profiler.h"

ChunkManager::ChunkManager(World* world) : m_world(world), m_chunks(), m_chunksVector(), m_newChunks(), m_oldChunks()
{
}

ChunkManager::~ChunkManager()
{
	for (unsigned int i = 0; i < m_chunksVector.size(); i++) {
		Chunk* chunk = m_chunksVector[i];
		delete chunk;
	}
}

void ChunkManager::update(float delta)
{
	Profiler::start("Updating chunks");
	if (getChunk(m_world->getCamera().getPosition() / (float)CHUNK_SIZE) == nullptr) {
		Vec3 cameraPosition = m_world->getCamera().getPosition() / (float)CHUNK_SIZE;
		generateChunk((int)floorf(cameraPosition.getX()), (int)floorf(cameraPosition.getY()), (int)floorf(cameraPosition.getZ()));
	}
	for (unsigned int i = 0; i < m_chunksVector.size(); i++) {
		m_chunksVector[i]->update(delta);
		Vec3 distanceToCamera = (m_world->getCamera().getPosition() - m_chunksVector[i]->getCenterPosition()) / (float)CHUNK_SIZE;
		if (distanceToCamera.length() > Vec3((float)(m_world->getVision() + 2.0f)).length()) {
			if (!m_chunksVector[i]->isRunning() && !m_chunksVector[i]->isRemoved()) {
				m_chunksVector[i]->remove();
				m_oldChunks.push_back(m_chunksVector[i]);
			}
		}
	}
	Profiler::stop();
	Profiler::start("Sorting chunks");
	for (unsigned int i = 0; i < m_newChunks.size(); i++) {
		if (m_newChunks[i]->isRemoved()) {
			m_newChunks[i] = m_newChunks.back();
			m_newChunks.pop_back();
			i--;
			continue;
		}
		m_newChunks[i]->calculateDistanceToCamera();
	}
	
	std::sort(m_newChunks.begin(), m_newChunks.end(), ChunkSorter());
	if (m_newChunks.size() > 0) {
		m_world->setMaxFogDistance(m_newChunks[0]->getDistanceToCamera() - Vec3(CHUNK_SIZE * 0.5f).length());
	}
	Profiler::stop();

	Profiler::start("Deleting chunks");
	for (unsigned int i = 0; i < m_oldChunks.size() && !ThreadManager::saturated(); i++) {
		printf("%d\n", m_oldChunks.size());
		Chunk* chunk = m_oldChunks[i];
		if (chunk->isRunning()) continue;
		if (chunk->isSaved()) {
			m_oldChunks[i] = m_oldChunks.back();
			m_oldChunks.pop_back();
			removeChunk(chunk);
			i--;
		} else {
			ThreadManager::addTask(ThreadTask("Chunk Save", chunkSaveThread, (void*)chunk));
		}
	}
	Profiler::stop();

	Profiler::start("Generating chunks");
	for (unsigned int i = 0; i < m_newChunks.size() && !ThreadManager::saturated(); i++) {
		Chunk* chunk = m_newChunks[i];
		if (chunk->isRunning()) continue;
		if (!chunk->isTerrainGenerated()) {
			ThreadManager::addTask(ThreadTask("Chunk Terrain", chunkTerrainThread, (void*)chunk));
		} else if (!chunk->isVertexDataCreated() && isChunkSurrounded(chunk->getX(), chunk->getY(), chunk->getZ())) {
			ThreadManager::addTask(ThreadTask("Chunk Mesh", chunkMeshThread, (void*)chunk));
		}
		if (chunk->isVertexDataCreated()) {
			m_newChunks[i] = m_newChunks.back();
			m_newChunks.pop_back();
		}
	}
	Profiler::stop();
}

void ChunkManager::render()
{
	for (unsigned int i = 0; i < m_chunksVector.size(); i++) {
		m_chunksVector[i]->render();
	}
}

Chunk* ChunkManager::generateChunk(int x, int y, int z)
{
	if ((m_world->getCamera().getPosition() - (Vec3(x + 0.5f, y + 0.5f, z + 0.5f) * CHUNK_SIZE)).length() < (m_world->getVision() + 1.0f) * CHUNK_SIZE) {
		try {
			Chunk* chunk = new Chunk(m_world, x, y, z);
			m_newChunks.push_back(chunk);
			addChunk(chunk);
			return chunk;
		} catch (std::bad_alloc& e) {
			std::cout << "Allocating chunk failed: " << e.what() << std::endl;
			system("PAUSE");
		}
	}
	return nullptr;
}

int chunkMeshThread(void* parameter)
{
	Chunk* chunk = (Chunk*)parameter;
	chunk->createMesh();
	return 0;
}

int chunkTerrainThread(void* parameter)
{
	Chunk* chunk = (Chunk*)parameter;
	chunk->generateTerrain();
	return 0;
}

int chunkSaveThread(void* parameter)
{
	Chunk* chunk = (Chunk*)parameter;
	chunk->save();
	return 0;
}

bool ChunkSorter::operator()(Chunk* i, Chunk* j)
{
	return (i->getDistanceToCamera() < j->getDistanceToCamera());
}

void ChunkManager::addChunk(Chunk* chunk)
{
	m_chunks[Vec3i(chunk->getX(), chunk->getY(), chunk->getZ())] = chunk;
	chunk->setIndex((int)m_chunksVector.size());
	m_chunksVector.push_back(chunk);
}

void ChunkManager::removeChunk(int x, int y, int z)
{
	Chunk* chunk = getChunk(x, y, z);
	m_chunks.erase(Vec3i(x, y, z));
	delete chunk;
}

Chunk* ChunkManager::getChunk(int x, int y, int z)
{
	auto it = m_chunks.find(Vec3i(x, y, z));
	if (it == m_chunks.end()) return nullptr;
	return it->second;
}

bool ChunkManager::isChunkTerrainGenerated(int x, int y, int z)
{
	Chunk* chunk = getChunk(x, y, z);
	if (chunk == nullptr) return false;
	return chunk->isTerrainGenerated();
}

bool ChunkManager::isChunkSurrounded(int x, int y, int z)
{
	return isChunkTerrainGenerated(x + 1, y, z) && isChunkTerrainGenerated(x, y + 1, z) && isChunkTerrainGenerated(x, y, z + 1) && isChunkTerrainGenerated(x - 1, y, z) && isChunkTerrainGenerated(x, y - 1, z) && isChunkTerrainGenerated(x, y, z - 1);
}

int ChunkManager::getBlock(int x, int y, int z)
{
	int cx = (int)floorf(x / (float)CHUNK_SIZE);
	int cy = (int)floorf(y / (float)CHUNK_SIZE);
	int cz = (int)floorf(z / (float)CHUNK_SIZE);

	Chunk* chunk = getChunk(cx, cy, cz);
	if (chunk == nullptr) return 0xffff00ff;

	int bx = x - cx * CHUNK_SIZE;
	int by = y - cy * CHUNK_SIZE;
	int bz = z - cz * CHUNK_SIZE;

	return chunk->getBlock(bx, by, bz);
}

void ChunkManager::setBlock(int x, int y, int z, unsigned int block)
{
	int cx = (int)floorf(x / (float)CHUNK_SIZE);
	int cy = (int)floorf(y / (float)CHUNK_SIZE);
	int cz = (int)floorf(z / (float)CHUNK_SIZE);

	Chunk* chunk = getChunk(cx, cy, cz);
	if (chunk == nullptr) return;

	int bx = x - cx * CHUNK_SIZE;
	int by = y - cy * CHUNK_SIZE;
	int bz = z - cz * CHUNK_SIZE;

	chunk->setBlock(bx, by, bz, block);
}

bool ChunkManager::isSolid(int x, int y, int z)
{
	return getBlock(x, y, z) != 0x00000000;
}

bool ChunkManager::pick(Vec3& position, Vec3& rotation, Vec3i& result)
{
	float sin = sinf(rotation.getY() * (float)M_PI / 180.0f);
	float cos = cosf(rotation.getY() * (float)M_PI / 180.0f);
	float ymul = cosf(rotation.getX() * (float)M_PI / 180.0f);

	Vec3 direction = Vec3(sin * ymul, -sinf(rotation.getX() * (float)M_PI / 180.0f), -cos * ymul) * 0.125f;
	Vec3 currentPosition = position;

	for (unsigned int i = 0; i < 64; i++) {
		Vec3i currentBlock(currentPosition);
		if (isSolid(currentBlock.getX(), currentBlock.getY(), currentBlock.getZ())) {
			result.set(currentBlock.getX(), currentBlock.getY(), currentBlock.getZ());
			return true;
		}
		currentPosition = currentPosition + direction;
	}
	return false;
}