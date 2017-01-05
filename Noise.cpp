#include "Noise.h"

#include <algorithm>
#include <random>

Noise::Noise(unsigned int seed) : m_p()
{
	m_p.resize(256);
	for (unsigned int i = 0; i < 256; i++) {
		m_p[i] = i;
	}
	std::default_random_engine engine(seed);
	std::shuffle(m_p.begin(), m_p.end(), engine);
	m_p.insert(m_p.end(), m_p.begin(), m_p.end());
}

Noise::~Noise()
{
}

float Noise::noise(float x, float y, float z)
{
	int xx = (int)floorf(x) & 255;
	int yy = (int)floorf(y) & 255;
	int zz = (int)floorf(z) & 255;
	
	x -= floorf(x);
	y -= floorf(y);
	z -= floorf(z);
	
	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	int a = m_p[xx] + yy;
	int aa = m_p[a] + zz;
	int ab = m_p[a + 1] + zz;
	int b = m_p[xx + 1] + yy;
	int ba = m_p[b] + zz;
	int bb = m_p[b + 1] + zz;

	return lerp(w, lerp(v, lerp(u, grad(m_p[aa], x, y, z), grad(m_p[ba], x - 1.0f, y, z)), lerp(u, grad(m_p[ab], x, y - 1.0f, z), grad(m_p[bb], x - 1.0f, y - 1.0f, z))), lerp(v, lerp(u, grad(m_p[aa + 1], x, y, z - 1.0f), grad(m_p[ba + 1], x - 1.0f, y, z - 1.0f)), lerp(u, grad(m_p[ab + 1], x, y - 1.0f, z - 1.0f), grad(m_p[bb + 1], x - 1.0f, y - 1.0f, z - 1.0f))));
}

float Noise::octaves(float x, float y, float z, unsigned int numOctaves, float scale)
{
	float currentValue = 0.0f;
	for (unsigned int i = 0; i < numOctaves; i++) {
		currentValue += noise(x, y, z);
		x *= scale;
		y *= scale;
		z *= scale;
	}
	return currentValue;
}

float Noise::fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float Noise::lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

float Noise::grad(int hash, float x, float y, float z)
{
	int h = hash & 15;
	float u = h < 8 ? x : y;
	float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}