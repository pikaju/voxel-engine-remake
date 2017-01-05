#pragma once

#include <vector>

class Noise
{
public:
	Noise(unsigned int seed);
	~Noise();

	float noise(float x, float y, float z);
	float octaves(float x, float y, float z, unsigned int numOctaves, float scale);
private:
	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);

	std::vector<short> m_p;
};

