#pragma once

#include <string>

#include "Vec2.h"
#include "Vec3.h"
#include "Mat4.h"

class Shader
{
public:
	Shader();
	~Shader();

	void init(const char* vertexFilePath, const char* fragmentFilePath);
	void cleanup();

	void enable();
	void disable();

	void setUniform(const std::string& name, int v);
	void setUniform(const std::string& name, float v);
	void setUniform(const std::string& name, Vec3& v);
	void setUniform(const std::string& name, Vec2& v);
	void setUniform(const std::string& name, Vec3* v, int nv);
	void setUniform(const std::string& name, Vec2* v, int nv);
	void setUniform(const std::string& name, Mat4& v);

	static inline Shader* getCurrentShader() { return m_currentShader; }
protected:
	bool loadShaderSource(const char* filePath, std::string* fileContents);
private:
	int m_vertexShader;
	int m_fragmentShader;
	int m_program;
	static Shader* m_currentShader;
};

