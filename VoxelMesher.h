#pragma once

#include "VertexData.h"
#include "Model.h"
#include "Chunk.h"

class Quad;

class VoxelMesher
{
public:
	static void meshChunk(VertexData& data, Chunk& chunk);
	static void meshChunkMarchingCubes(VertexData& data, Chunk& chunk);
protected:
	static void optimizeQuads(std::vector<Quad>& quads);
	static int countMatches(Vec3* v0, Vec3* v1);
};

class Quad
{
public:
	Quad(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3, Color color) : m_p0(p0), m_p1(p1), m_p2(p2), m_p3(p3), m_color(color) {  }
	Quad(Quad& q0, Quad& q1) : m_p0(), m_p1(), m_p2(), m_p3(), m_color(q0.getColor())
	{
		std::vector<Vec3> p;
		p.push_back(q0.getP0());
		p.push_back(q1.getP0());
		p.push_back(q0.getP1());
		p.push_back(q1.getP1());
		p.push_back(q0.getP2());
		p.push_back(q1.getP2());
		p.push_back(q0.getP3());
		p.push_back(q1.getP3());
		for (unsigned int i = 0; i < p.size(); i++) {
			for (unsigned int j = i + 1; j < p.size(); j++) {
				if (p[i] == p[j]) {
					p.erase(p.begin() + j);
					p.erase(p.begin() + i);
					i--;
					break;
				}
			}
		}
		m_p0 = p[0];
		m_p1 = p[1];
		m_p2 = p[2];
		m_p3 = p[3];
	}
	void increaseSize(float amount)
	{
		if (m_p0.getX() > m_p1.getX() || m_p0.getX() > m_p2.getX() || m_p0.getX() > m_p3.getX()) m_p0.setX(m_p0.getX() + amount);
		if (m_p1.getX() > m_p0.getX() || m_p1.getX() > m_p2.getX() || m_p1.getX() > m_p3.getX()) m_p1.setX(m_p1.getX() + amount);
		if (m_p2.getX() > m_p0.getX() || m_p2.getX() > m_p1.getX() || m_p2.getX() > m_p3.getX()) m_p2.setX(m_p2.getX() + amount);
		if (m_p3.getX() > m_p0.getX() || m_p3.getX() > m_p1.getX() || m_p3.getX() > m_p2.getX()) m_p3.setX(m_p3.getX() + amount);

		if (m_p0.getY() > m_p1.getY() || m_p0.getY() > m_p2.getY() || m_p0.getY() > m_p3.getY()) m_p0.setY(m_p0.getY() + amount);
		if (m_p1.getY() > m_p0.getY() || m_p1.getY() > m_p2.getY() || m_p1.getY() > m_p3.getY()) m_p1.setY(m_p1.getY() + amount);
		if (m_p2.getY() > m_p0.getY() || m_p2.getY() > m_p1.getY() || m_p2.getY() > m_p3.getY()) m_p2.setY(m_p2.getY() + amount);
		if (m_p3.getY() > m_p0.getY() || m_p3.getY() > m_p1.getY() || m_p3.getY() > m_p2.getY()) m_p3.setY(m_p3.getY() + amount);

		if (m_p0.getZ() > m_p1.getZ() || m_p0.getZ() > m_p2.getZ() || m_p0.getZ() > m_p3.getZ()) m_p0.setZ(m_p0.getZ() + amount);
		if (m_p1.getZ() > m_p0.getZ() || m_p1.getZ() > m_p2.getZ() || m_p1.getZ() > m_p3.getZ()) m_p1.setZ(m_p1.getZ() + amount);
		if (m_p2.getZ() > m_p0.getZ() || m_p2.getZ() > m_p1.getZ() || m_p2.getZ() > m_p3.getZ()) m_p2.setZ(m_p2.getZ() + amount);
		if (m_p3.getZ() > m_p0.getZ() || m_p3.getZ() > m_p1.getZ() || m_p3.getZ() > m_p2.getZ()) m_p3.setZ(m_p3.getZ() + amount);
	}
	inline Vec3& getP0() { return m_p0; }
	inline Vec3& getP1() { return m_p1; }
	inline Vec3& getP2() { return m_p2; }
	inline Vec3& getP3() { return m_p3; }
	inline Color& getColor() { return m_color; }
private:
	Vec3 m_p0;
	Vec3 m_p1;
	Vec3 m_p2;
	Vec3 m_p3;
	Color m_color;
};
