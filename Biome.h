#pragma once

#include "Noise.h"

class Chunk;

class Biome
{
public:
	static void generateChunk(Chunk* chunk);
	static bool isGround(Noise& noise, float x, float y, float z);
	static bool isCave(Noise& noise, float x, float y, float z);
};

