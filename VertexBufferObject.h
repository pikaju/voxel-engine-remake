#pragma once

#include "Model.h"

class VertexBufferObject : public Model
{
public:
	VertexBufferObject();
	~VertexBufferObject();

	void init();
	void cleanup();

	void setData(Vertex* vertexData, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void render();
private:
	unsigned int m_vbo;
	unsigned int m_ibo;
	unsigned int m_vao;

	unsigned int m_numIndices;
};

