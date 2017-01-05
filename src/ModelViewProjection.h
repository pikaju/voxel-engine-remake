#pragma once

#include "Mat4.h"

class ModelViewProjection
{
public:
	ModelViewProjection();
	~ModelViewProjection();

	void identity();
	Mat4 getMVP() { return m_modelMatrix * m_viewMatrix * m_projectionMatrix; }
private:
	Mat4 m_modelMatrix;
	Mat4 m_viewMatrix;
	Mat4 m_projectionMatrix;
};

