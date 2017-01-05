#pragma once
class VoxelArrayContainer
{
public:
	VoxelArrayContainer();
	~VoxelArrayContainer();

	unsigned int getBlock(int index);
	void getBlocks(unsigned int* blocks);
	void setBlock(int index, unsigned int block);
	void setBlocks(unsigned int* blocks);
private:
	unsigned int* m_data;
};

