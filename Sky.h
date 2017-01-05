#pragma once

#include "Shader.h"
#include "Texture.h"

class Sky
{
public:
	Sky();
	~Sky();
	
	void init(unsigned int width, unsigned int height);
	void render(Camera& camera, Mat4& projectionMatrix);

	inline Texture& getRenderTarget() { return m_renderTarget; }
private:
	Shader m_shader;
	Texture m_renderTarget;

	Vec3 m_sunDirection;
};

