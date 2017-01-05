#pragma once

#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Light.h"

class Lighting
{
public:
	Lighting();
	~Lighting();

	void init(unsigned int width, int unsigned height);

	void render(Texture& worldTarget, Vec3& cameraPosition, float renderDistance);

	inline void addLight(Light& light) { m_lights.push_back(light); }
	inline Texture& getRenderTarget() { return m_renderTarget; }
protected:
	void setUniform(std::string& name, Light& light);
private:
	Shader m_shader;
	Texture m_renderTarget;
	std::vector<Light> m_lights;
};

