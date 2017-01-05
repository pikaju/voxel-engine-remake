#pragma once

#include <vector>

#include "DataContainer.h"

struct VoxelContainerData
{
	VoxelContainerData(unsigned int block = 0, int amount = 0) : m_block(block), m_amount(amount) {  }
	unsigned int m_block;
	int m_amount;
};

class VoxelContainer
{
public:
	VoxelContainer();
	~VoxelContainer();

	unsigned int getBlock(int index);
	void getBlocks(unsigned int* blocks);
	void setBlock(int index, unsigned int block);
	void setBlocks(unsigned int* blocks);
	void optimize();

	void getData(DataContainer& container);
private:
	std::vector<VoxelContainerData> m_data;
};

