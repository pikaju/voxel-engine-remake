#pragma once

#include "VertexData.h"

class Model
{
public:
	Model();
	~Model();

	virtual void init();
	virtual void cleanup();

	virtual void setData(Vertex* vertexData, unsigned int numVertices, unsigned int* indices, unsigned int numIndices) = 0;
	virtual void render() = 0;
};

