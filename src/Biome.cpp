#include "Biome.h"

#include "World.h"

void Biome::generateChunk(Chunk* chunk)
{
	Noise& noise = chunk->getWorld()->getNoise();
	unsigned int blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				float x = (float)(i + chunk->getX() * CHUNK_SIZE);
				float y = (float)(j + chunk->getY() * CHUNK_SIZE);
				float z = (float)(k + chunk->getZ() * CHUNK_SIZE);
				
				if (isGround(noise, x, y, z)) {
					if (isGround(noise, x, y + 2.0f, z)) {
						blocks[i + j * CHUNK_SIZE + k * CHUNK_SIZE * CHUNK_SIZE] = 0xff303030;
					} else {
						blocks[i + j * CHUNK_SIZE + k * CHUNK_SIZE * CHUNK_SIZE] = 0xff155015;
					}
				} else {
					blocks[i + j * CHUNK_SIZE + k * CHUNK_SIZE * CHUNK_SIZE] = 0x00000000;
				}
				//if (isCave(noise, x, y, z)) {
				//chunk->setBlock(i, j, k, 0x00000000);
					//}
			}
		}
	}
	chunk->setBlocks(blocks);
}

bool Biome::isGround(Noise& noise, float x, float y, float z)
{
	const float scale = 0.0625f * 0.0625f;
	float n = (noise.noise(x * scale / 0.0625f, y * scale / 0.0625f, z * scale / 0.0625f) * 0.0625f + noise.noise(x * scale, y * scale, z * scale) + noise.noise(x * scale / 0.25f, y * scale / 0.25f, z * scale / 0.25f) * 0.25f);
	return n > y / (1024.0f * powf(noise.noise(x * scale * 0.25f, y * scale * 0.25f, z * scale * 0.25f) * 0.5f + 0.5f, 4.0f));
}

bool Biome::isCave(Noise& noise, float x, float y, float z)
{
	const float scale = 0.01f;
	float d = noise.noise(x * scale, y * scale, z * scale) + noise.noise((x + 64.0f) * scale, y * scale, z * scale);
	return d > 0.0f && d < 0.1f;
}