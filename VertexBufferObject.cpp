#include "VertexBufferObject.h"

#include <glew.h>

VertexBufferObject::VertexBufferObject() : m_vbo(-1), m_ibo(-1), m_vao(-1)
{
}

VertexBufferObject::~VertexBufferObject()
{
}

void VertexBufferObject::init()
{
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glGenVertexArrays(1, &m_vao);
}

void VertexBufferObject::cleanup()
{
	if (m_vbo != -1) glDeleteBuffers(1, &m_vbo);
	if (m_ibo != -1) glDeleteBuffers(1, &m_ibo);
	if (m_vao != -1) glDeleteVertexArrays(1, &m_vao);
	m_vbo = -1;
	m_ibo = -1;
	m_vao = -1;
}

void VertexBufferObject::setData(Vertex* vertexData, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	m_numIndices = numIndices;
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);
	
	glBindVertexArray(m_vao);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, m_position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)offsetof(Vertex, m_color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexBufferObject::render()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}