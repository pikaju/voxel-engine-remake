#include "VoxelArrayContainer.h"

#include "Chunk.h"

VoxelArrayContainer::VoxelArrayContainer()
{
	m_data = new unsigned int[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
}


VoxelArrayContainer::~VoxelArrayContainer()
{
	delete[] m_data;
}

unsigned int VoxelArrayContainer::getBlock(int index)
{
	return m_data[index];
}

void VoxelArrayContainer::getBlocks(unsigned int* blocks)
{
	memcpy(blocks, m_data, sizeof(unsigned int) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
}

void VoxelArrayContainer::setBlock(int index, unsigned int block)
{
	m_data[index] = block;
}

void VoxelArrayContainer::setBlocks(unsigned int* blocks)
{
	memcpy(m_data, blocks, sizeof(unsigned int) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
}