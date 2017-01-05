#pragma once

#include "Shader.h"
#include "Texture.h"

class FXAA
{
public:
	FXAA();
	~FXAA();
	void init();
	void render(Texture& texture);
private:
	Shader m_shader;
};

