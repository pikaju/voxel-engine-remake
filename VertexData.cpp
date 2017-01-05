#include "VertexData.h"

#include "VertexBufferObject.h"
#include "Model.h"

VertexData::VertexData() : m_vertexData(), m_indices()
{
}

VertexData::~VertexData()
{
}

void VertexData::quad(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3, Color color)
{
	unsigned int vertexSize = m_vertexData.size();
	m_indices.push_back(vertexSize + 0);
	m_indices.push_back(vertexSize + 1);
	m_indices.push_back(vertexSize + 2);
	m_indices.push_back(vertexSize + 0);
	m_indices.push_back(vertexSize + 2);
	m_indices.push_back(vertexSize + 3);

	Vec3 normal = (p2 - p0).cross(p2 - p1).normalized();
	m_vertexData.push_back(Vertex(p0, color, normal));
	m_vertexData.push_back(Vertex(p1, color, normal));
	m_vertexData.push_back(Vertex(p2, color, normal));
	m_vertexData.push_back(Vertex(p3, color, normal));
}

void VertexData::triangle(Vec3 p0, Vec3 p1, Vec3 p2, Color color)
{
	unsigned int vertexSize = m_vertexData.size();
	m_indices.push_back(vertexSize + 0);
	m_indices.push_back(vertexSize + 1);
	m_indices.push_back(vertexSize + 2);

	Vec3 normal = (p2 - p0).cross(p2 - p1).normalized();
	m_vertexData.push_back(Vertex(p0, color, normal));
	m_vertexData.push_back(Vertex(p1, color, normal));
	m_vertexData.push_back(Vertex(p2, color, normal));
}

Model* VertexData::createModel()
{
	Model* model = new VertexBufferObject();
	model->init();
	model->setData(m_vertexData.data(), m_vertexData.size(), m_indices.data(), m_indices.size());
	return model;
}

void VertexData::calculateNormals()
{
	for (unsigned int i = 0; i < m_indices.size(); i += 3) {
		Vertex& v1 = m_vertexData[m_indices[i]];
		Vertex& v2 = m_vertexData[m_indices[i + 1]];
		Vertex& v3 = m_vertexData[m_indices[i + 2]];

		Vec3 normal = ((v3.m_position - v1.m_position).normalized()).cross((v3.m_position - v2.m_position).normalized()).normalized();

		v1.m_normal = v1.m_normal + normal;
		v2.m_normal = v2.m_normal + normal;
		v3.m_normal = v3.m_normal + normal;
	}
	for (unsigned int i = 0; i < m_vertexData.size(); i++) {
		m_vertexData[i].m_normal.normalize();
	}
}

void VertexData::clear()
{
	m_vertexData.clear();
	m_indices.clear();
}