#pragma once

#include <math.h>

#include "Vec3.h"
#include "Mat4.h"

class FrustumPlane
{
public:
	FrustumPlane(float a, float b, float c, float d) : m_a(a), m_b(b), m_c(c), m_d(d) { normalize(); }
	FrustumPlane() : m_a(0.0f), m_b(0.0f), m_c(0.0f), m_d(0.0f) {}
	void normalize()
	{
		float t = sqrtf(m_a * m_a + m_b * m_b + m_c * m_c);
		m_a /= t;
		m_b /= t;
		m_c /= t;
		m_d /= t;
	}

	inline float getA() { return m_a; }
	inline float getB() { return m_b; }
	inline float getC() { return m_c; }
	inline float getD() { return m_d; }
private:
	float m_a;
	float m_b;
	float m_c;
	float m_d;
};

class FrustumCuller
{
public:
	static void extractPlanes(Mat4& matrix);
	static bool isSphereInFrustum(Vec3& position, float radius);
private:
	static FrustumPlane planes[6];
};

