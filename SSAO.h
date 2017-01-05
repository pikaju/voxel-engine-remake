#pragma once

#include "Texture.h"
#include "Shader.h"
#include "Vec2.h"

class SSAO
{
public:
	SSAO(unsigned int numSamples = 16);
	~SSAO();

	void init(unsigned int width, unsigned int height);

	void render(Texture& worldTarget, Vec3& cameraPosition);

	inline Texture& getRenderTarget() { return m_renderTarget; }
private:
	Texture m_renderTarget;
	Shader m_shader;
	Vec2* m_sampleKernel;
	unsigned int m_numSamples;
};

