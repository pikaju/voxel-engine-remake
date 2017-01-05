#include "Chunk.h"

#include <SDL_timer.h>
#include <glew.h>

#include "World.h"
#include "VoxelMesher.h"
#include "Biome.h"
#include "FrustumCuller.h"
#include "Save.h"

Chunk::Chunk(World* world, int x, int y, int z) : m_world(world), m_x(x), m_y(y), m_z(z), m_blocks(), m_numNeighbors(0), m_vertexData(), m_model(nullptr), m_modelGenerated(false), m_vertexDataGenerated(false), m_generatingVertexData(false), m_terrainGenerated(false), m_running(false), m_removed(false), m_saved(true)
{
	ChunkManager& chunkManager = m_world->getChunkManager();
	m_neighbors[ChunkDirection::RIGHT] = chunkManager.getChunk(m_x + 1, m_y, m_z);
	if (m_neighbors[ChunkDirection::RIGHT] != nullptr) m_neighbors[ChunkDirection::RIGHT]->setNeighbor(ChunkDirection::LEFT, this);
	m_neighbors[ChunkDirection::LEFT] = chunkManager.getChunk(m_x - 1, m_y, m_z);
	if (m_neighbors[ChunkDirection::LEFT] != nullptr) m_neighbors[ChunkDirection::LEFT]->setNeighbor(ChunkDirection::RIGHT, this);
	m_neighbors[ChunkDirection::UP] = chunkManager.getChunk(m_x, m_y + 1, m_z);
	if (m_neighbors[ChunkDirection::UP] != nullptr) m_neighbors[ChunkDirection::UP]->setNeighbor(ChunkDirection::DOWN, this);
	m_neighbors[ChunkDirection::DOWN] = chunkManager.getChunk(m_x, m_y - 1, m_z);
	if (m_neighbors[ChunkDirection::DOWN] != nullptr) m_neighbors[ChunkDirection::DOWN]->setNeighbor(ChunkDirection::UP, this);
	m_neighbors[ChunkDirection::FRONT] = chunkManager.getChunk(m_x, m_y, m_z + 1);
	if (m_neighbors[ChunkDirection::FRONT] != nullptr) m_neighbors[ChunkDirection::FRONT]->setNeighbor(ChunkDirection::BACK, this);
	m_neighbors[ChunkDirection::BACK] = chunkManager.getChunk(m_x, m_y, m_z - 1);
	if (m_neighbors[ChunkDirection::BACK] != nullptr) m_neighbors[ChunkDirection::BACK]->setNeighbor(ChunkDirection::FRONT, this);
	for (int i = 0; i < 6; i++) {
		if (m_neighbors[i] != nullptr) m_numNeighbors++;
	}
}

Chunk::~Chunk()
{
	if (m_neighbors[ChunkDirection::RIGHT] != nullptr) m_neighbors[ChunkDirection::RIGHT]->removeNeighbor(ChunkDirection::LEFT);
	if (m_neighbors[ChunkDirection::LEFT] != nullptr) m_neighbors[ChunkDirection::LEFT]->removeNeighbor(ChunkDirection::RIGHT);
	if (m_neighbors[ChunkDirection::UP] != nullptr) m_neighbors[ChunkDirection::UP]->removeNeighbor(ChunkDirection::DOWN);
	if (m_neighbors[ChunkDirection::DOWN] != nullptr) m_neighbors[ChunkDirection::DOWN]->removeNeighbor(ChunkDirection::UP);
	if (m_neighbors[ChunkDirection::FRONT] != nullptr) m_neighbors[ChunkDirection::FRONT]->removeNeighbor(ChunkDirection::BACK);
	if (m_neighbors[ChunkDirection::BACK] != nullptr) m_neighbors[ChunkDirection::BACK]->removeNeighbor(ChunkDirection::FRONT);
	
	std::vector<Chunk*>& chunks = m_world->getChunkManager().getChunks();
	chunks[m_index] = chunks.back();
	chunks[m_index]->setIndex(m_index);
	chunks.pop_back();

	deleteModel();
}

void Chunk::update(float delta)
{
	if (isTerrainGenerated() && m_numNeighbors < 6) {
		ChunkManager& chunkManager = m_world->getChunkManager();
		if (m_neighbors[ChunkDirection::RIGHT] == nullptr) chunkManager.generateChunk(m_x + 1, m_y, m_z);
		if (m_neighbors[ChunkDirection::LEFT] == nullptr) chunkManager.generateChunk(m_x - 1, m_y, m_z);
		if (m_neighbors[ChunkDirection::UP] == nullptr) chunkManager.generateChunk(m_x, m_y + 1, m_z);
		if (m_neighbors[ChunkDirection::DOWN] == nullptr) chunkManager.generateChunk(m_x, m_y - 1, m_z);
		if (m_neighbors[ChunkDirection::FRONT] == nullptr) chunkManager.generateChunk(m_x, m_y, m_z + 1);
		if (m_neighbors[ChunkDirection::BACK] == nullptr) chunkManager.generateChunk(m_x, m_y, m_z - 1);
	}
}

void Chunk::render()
{
	createModel();
	if (isModelCreated() && m_model) {
		if (FrustumCuller::isSphereInFrustum(getCenterPosition(), Vec3((float)CHUNK_SIZE / 2.0f).length())) m_model->render();
	}
}

void Chunk::createMesh()
{
	m_running = true;
	if (m_generatingVertexData) {
		m_running = false;
		return;
	}
	m_generatingVertexData = true;
	m_vertexData.clear();
	VoxelMesher::meshChunk(m_vertexData, *this);
	m_vertexDataGenerated = true;
	m_running = false;
}

void Chunk::createModel()
{
	if (!isModelCreated() && m_vertexDataGenerated) {
		m_modelGenerated = true;
		if (m_vertexData.getSize() > 0) {
			m_model = m_vertexData.createModel();
			m_vertexData.clear();
		}
		m_vertexDataGenerated = false;
		m_generatingVertexData = false;
	}
}

void Chunk::deleteModel()
{
	if (isModelCreated()) {
		m_modelGenerated = false;
		if (m_model != nullptr) {
			m_model->cleanup();
			delete m_model;
			m_model = nullptr;
		}
	}
}

void Chunk::recreateMesh()
{
	deleteModel();
	createMesh();
	createModel();
}

void Chunk::recreateNeighborMesh(int x, int y, int z)
{
	if (x == 0 && m_neighbors[LEFT] != nullptr) m_neighbors[LEFT]->recreateMesh();
	if (y == 0 && m_neighbors[DOWN] != nullptr) m_neighbors[DOWN]->recreateMesh();
	if (z == 0 && m_neighbors[BACK] != nullptr) m_neighbors[BACK]->recreateMesh();
	if (x == CHUNK_SIZE - 1 && m_neighbors[RIGHT] != nullptr) m_neighbors[RIGHT]->recreateMesh();
	if (y == CHUNK_SIZE - 1 && m_neighbors[UP] != nullptr) m_neighbors[UP]->recreateMesh();
	if (z == CHUNK_SIZE - 1 && m_neighbors[FRONT] != nullptr) m_neighbors[FRONT]->recreateMesh();
}

void Chunk::generateTerrain()
{
	m_running = true;
	Biome::generateChunk(this);
	m_terrainGenerated = true;
	m_running = false;
}

unsigned int Chunk::getBlock(int x, int y, int z)
{
	return m_blocks.getBlock(x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE);
}

void Chunk::getBlocks(unsigned int* blocks)
{
	m_blocks.getBlocks(blocks);
}

void Chunk::setBlock(int x, int y, int z, unsigned int block)
{
	m_blocks.setBlock(x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE, block);
	recreateMesh();
	recreateNeighborMesh(x, y, z);
	m_saved = false;
}

void Chunk::setBlocks(unsigned int* blocks)
{
	m_blocks.setBlocks(blocks);
	m_saved = false;
}

void Chunk::setNeighbor(int index, Chunk* neighbor)
{
	m_neighbors[index] = neighbor;
	m_numNeighbors++;
}

void Chunk::removeNeighbor(int index)
{
	m_neighbors[index] = nullptr;
	m_numNeighbors--;
}

void Chunk::calculateDistanceToCamera()
{
	m_distanceToCamera = (getCenterPosition() - m_world->getCamera().getPosition()).length();
}

void Chunk::save()
{
	m_running = true;
	Save::openOutput((std::string("Resources/World/") + std::to_string(m_x) + ";" + std::to_string(m_y) + ";" + std::to_string(m_z)).c_str());
	DataContainer container;
	m_blocks.getData(container);
	Save::writeDataContainer(container);
	Save::closeOutput();
	m_saved = true;
	m_running = false;
}