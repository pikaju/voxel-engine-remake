#pragma once

#include <unordered_map>
#include <vector>

#include "Chunk.h"
#include "Vec3i.h"

class World;

class ChunkManager
{
public:
	ChunkManager(World* world);
	~ChunkManager();
	
	void update(float delta);
	void render();

	Chunk* generateChunk(int x, int y, int z);
	void addChunk(Chunk* chunk);
	void removeChunk(int x, int y, int z);
	void removeChunk(Chunk* chunk) { removeChunk(chunk->getX(), chunk->getY(), chunk->getZ()); }
	Chunk* getChunk(int x, int y, int z);
	inline Chunk* getChunk(Vec3& position) { return getChunk((int)floorf(position.getX()), (int)floorf(position.getY()), (int)floorf(position.getZ())); }
	inline std::vector<Chunk*>& getChunks() { return m_chunksVector; }
	inline std::vector<Chunk*>& getNewChunks() { return m_newChunks; }
	bool isChunkTerrainGenerated(int x, int y, int z);
	bool isChunkSurrounded(int x, int y, int z);
	int getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, unsigned int block);
	bool isSolid(int x, int y, int z);
	bool ChunkManager::pick(Vec3& position, Vec3& rotation, Vec3i& result);

	inline World* getWorld() { return m_world; }
private:
	World* m_world;

	std::unordered_map<Vec3i, Chunk*> m_chunks;
	std::vector<Chunk*> m_chunksVector;
	std::vector<Chunk*> m_newChunks;
	std::vector<Chunk*> m_oldChunks;
};

class ChunkSorter
{
public:
	ChunkSorter() {  }
	~ChunkSorter() {  }
	bool operator()(Chunk* i, Chunk* j);
};

extern int chunkTerrainThread(void* parameter);
extern int chunkMeshThread(void* parameter);
extern int chunkSaveThread(void* parameter);
