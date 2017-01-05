#include "VoxelContainer.h"

#include "Chunk.h"

VoxelContainer::VoxelContainer()
{
	m_data.emplace_back(0, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
}

VoxelContainer::~VoxelContainer()
{
}

unsigned int VoxelContainer::getBlock(int index)
{
	int currentIndex = 0;
	for (unsigned int i = 0; i < m_data.size(); i++) {
		currentIndex += m_data[i].m_amount;
		if (currentIndex > index) {
			return m_data[i].m_block;
		}
	}
	return 0xffff00ff;
}

void VoxelContainer::getBlocks(unsigned int* blocks)
{
	unsigned int currentIndex = 0;
	for (unsigned int i = 0; i < m_data.size(); i++) {
		for (int j = 0; j < m_data[i].m_amount && currentIndex < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; j++) {
			blocks[currentIndex] = m_data[i].m_block;
			currentIndex++;
		}
	}
}

void VoxelContainer::setBlock(int index, unsigned int block)
{
	int currentIndex = 0;
	for (unsigned int i = 0; i < m_data.size(); i++) {
		currentIndex += m_data[i].m_amount;
		if (currentIndex >= index) {
			m_data[i].m_amount -= currentIndex - index;
			unsigned int blockType = m_data[i].m_block;
			if (currentIndex - index > 0) m_data.insert(m_data.begin() + i + 1, VoxelContainerData(blockType, currentIndex - index));
			m_data[i + 1].m_amount--;
			m_data.insert(m_data.begin() + i + 1, VoxelContainerData(block, 1));
			break;
		}
	}
	optimize();
}

void VoxelContainer::setBlocks(unsigned int* blocks)
{
	m_data.clear();
	for (unsigned int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;) {
		unsigned int block = blocks[i];
		int amount = i + 1;
		while (amount < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE && blocks[amount] == block) amount++;
		m_data.emplace_back(block, amount - i);
		i += amount - i;
	}
}

void VoxelContainer::optimize()
{
	for (unsigned int i = 1; i < m_data.size(); i++) {
		if (m_data[i].m_block == m_data[i - 1].m_block) {
			m_data[i - 1].m_amount += m_data[i].m_amount;
			m_data.erase(m_data.begin() + i);
			i--;
		}
		if (m_data[i].m_amount <= 0) {
			m_data.erase(m_data.begin() + i);
			i--;
		}
	}
}

void VoxelContainer::getData(DataContainer& container)
{
	container.addUnsignedInt(m_data.size());
	for (unsigned int i = 0; i < m_data.size(); i++) {
		container.addUnsignedInt(m_data[i].m_block);
		container.addInt(m_data[i].m_amount);
	}
}