#pragma once

#include <vector>

#include "Vec3.h"

class Model;

struct Color
{
	unsigned char m_r;
	unsigned char m_g;
	unsigned char m_b;
	unsigned char m_a;
	
	Color(Vec3& color)
	{
		m_r = (unsigned char)(color.getX() * 255.0f);
		m_g = (unsigned char)(color.getY() * 255.0f);
		m_b = (unsigned char)(color.getZ() * 255.0f);
		m_a = 255;
	}
	Color(unsigned int color)
	{
		m_a = (unsigned char)(color >> 24 & 0xff);
		m_r = (unsigned char)(color >> 16 & 0xff);
		m_g = (unsigned char)(color >> 8 & 0xff);
		m_b = (unsigned char)(color & 0xff);
	}
	bool operator==(Color& v)
	{
		return m_r == v.m_r && m_g == v.m_g && m_b == v.m_b && m_a == v.m_a;
	}
};

struct Vertex
{
	Vec3 m_position;
	Color m_color;
	Vec3 m_normal;

	Vertex(Vec3& position, Color& color, Vec3& normal = Vec3()) : m_position(position), m_color(color), m_normal(normal) {  }
};

class VertexData
{
public:
	VertexData();
	~VertexData();

	void quad(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3, Color color);
	void triangle(Vec3 p0, Vec3 p1, Vec3 p2, Color color);
	Model* createModel();
	void calculateNormals();

	inline void addVertex(Vertex& vertex) { m_vertexData.push_back(vertex); }
	inline void addIndex(unsigned int index) { m_indices.push_back(index); }

	void clear();
	inline int getNumVertices() { return m_vertexData.size(); }
	inline int getSize() { return m_indices.size(); }
private:
	std::vector<Vertex> m_vertexData;
	std::vector<unsigned int> m_indices;
};

