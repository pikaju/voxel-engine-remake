#pragma once

#include "Texture.h"
#include "Shader.h"
#include "Mat4.h"

class World;

class Shadow
{
public:
	Shadow();
	~Shadow();

	void init(int shadowMapSize, float shadowMapDensity);

	void render(World* world);

	inline Texture& getRenderTarget() { return m_renderTarget; }
	inline Mat4& getMVPMatrix() { return m_mvpMatrix; }
private:
	Texture m_renderTarget;
	Shader m_shader;
	Mat4 m_mvpMatrix;
	float m_shadowMapDensity;
};

