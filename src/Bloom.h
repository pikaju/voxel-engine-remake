#pragma once

#include "Shader.h"
#include "Texture.h"

class Bloom
{
public:
	Bloom();
	~Bloom();

	void init(unsigned int width, unsigned int height);
	void render(Texture& texture);

	inline Texture& getRenderTarget() { return m_renderTarget; }
private:
	Shader m_shader;
	Texture m_renderTarget;
};

