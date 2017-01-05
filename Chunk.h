#pragma once

#define CHUNK_SIZE 32

#include "VertexData.h"
#include "VoxelContainer.h"

class World;

enum ChunkDirection {
	RIGHT, LEFT, UP, DOWN, FRONT, BACK
};

class Chunk
{
public:
	Chunk(World* world, int x, int y, int z);
	~Chunk();

	void update(float delta);
	void render();

	void createMesh();
	void createModel();
	void deleteModel();
	void recreateMesh();
	void generateTerrain();

	void recreateNeighborMesh(int x, int y, int z);

	inline World* getWorld() { return m_world; }

	unsigned int getBlock(int x, int y, int z);
	void getBlocks(unsigned int* blocks);
	void setBlock(int x, int y, int z, unsigned int block);
	void setBlocks(unsigned int* blocks);

	inline int getX() { return m_x; }
	inline int getY() { return m_y; }
	inline int getZ() { return m_z; }
	inline Vec3 getCenterPosition() { return Vec3((float)(m_x * CHUNK_SIZE + CHUNK_SIZE / 2), (float)(m_y * CHUNK_SIZE + CHUNK_SIZE / 2), (float)(m_z * CHUNK_SIZE + CHUNK_SIZE / 2)); }
	inline bool isModelCreated() { return m_modelGenerated; }
	inline bool isVertexDataCreated() { return m_vertexDataGenerated; }
	inline bool isGeneratingVertexData() { return m_generatingVertexData; }
	inline bool isTerrainGenerated() { return m_terrainGenerated; }
	inline bool isRunning() { return m_running; }
	inline float getDistanceToCamera() { return m_distanceToCamera; }
	inline Chunk* getNeighbor(int index) { return m_neighbors[index]; }
	void setNeighbor(int index, Chunk* neighbor);
	void removeNeighbor(int index);
	void calculateDistanceToCamera();
	inline void setIndex(int index) { m_index = index; }

	void save();

	inline bool isRemoved() { return m_removed; }
	inline void remove() { m_removed = true; }
	inline bool isSaved() { return m_saved; }
private:
	World* m_world;
	VoxelContainer m_blocks;
	int m_x;
	int m_y;
	int m_z;

	float m_distanceToCamera;

	Chunk* m_neighbors[6];
	int m_numNeighbors;

	VertexData m_vertexData;
	Model* m_model;

	int m_index;
	
	bool m_modelGenerated;
	bool m_vertexDataGenerated;
	bool m_generatingVertexData;
	bool m_terrainGenerated;
	bool m_running;
	bool m_removed;
	bool m_saved;
};

