#include "ModelViewProjection.h"


ModelViewProjection::ModelViewProjection()
{
}

ModelViewProjection::~ModelViewProjection()
{
}

void ModelViewProjection::identity()
{
	m_modelMatrix.identity();
	m_viewMatrix.identity();
	m_projectionMatrix.identity();
}