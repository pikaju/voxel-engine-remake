#include "Shader.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <glew.h>

Shader* Shader::m_currentShader = nullptr;

Shader::Shader()
{
}

Shader::~Shader()
{
	cleanup();
}

void Shader::init(const char* vertexFilePath, const char* fragmentFilePath)
{
	m_program = glCreateProgram();

	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	std::string vertexShaderSourceString;
	std::string fragmentShaderSourceString;

	if (!loadShaderSource(vertexFilePath, &vertexShaderSourceString)) {
		std::cout << "Failed to load " << vertexFilePath << std::endl;
		return;
	}
	if (!loadShaderSource(fragmentFilePath, &fragmentShaderSourceString)) {
		std::cout << "Failed to load " << fragmentFilePath << std::endl;
		return;
	}

	const char* vertexShaderSource = vertexShaderSourceString.c_str();
	const char* fragmentShaderSource = fragmentShaderSourceString.c_str();
	
	glShaderSource(m_vertexShader, 1, &vertexShaderSource, nullptr);
	glShaderSource(m_fragmentShader, 1, &fragmentShaderSource, nullptr);

	int compileStatus;
	int maxLength;

	glCompileShader(m_vertexShader);
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(m_vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
	std::vector<char> vertexInfoLog(maxLength);
	glGetShaderInfoLog(m_vertexShader, maxLength, &maxLength, &vertexInfoLog[0]);
	if (compileStatus == GL_FALSE) {
		std::cout << "Failed to compile " << vertexFilePath << ":" << std::endl << vertexInfoLog.data() << std::endl;
	}

	glCompileShader(m_fragmentShader);
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(m_fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
	std::vector<char> fragmentInfoLog(maxLength);
	glGetShaderInfoLog(m_fragmentShader, maxLength, &maxLength, &fragmentInfoLog[0]);
	if (compileStatus == GL_FALSE) {
		std::cout << "Failed to compile " << fragmentFilePath << ":" << std::endl << fragmentInfoLog.data() << std::endl;
	}

	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);

	int success;
	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);
		if (maxLength == 0) std::cout << "Failed to link" << vertexFilePath << " and " << fragmentFilePath << ", no info log" << std::endl;
		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]);
		std::cout << "Failed to link " << vertexFilePath << " and " << fragmentFilePath << ":" << std::endl << infoLog.data() << std::endl;
	}

	glValidateProgram(m_program);
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &success);
	if (success == GL_FALSE) {
		std::cout << "Failed to validate " << vertexFilePath << " and " << fragmentFilePath << std::endl;
	}
}

void Shader::cleanup()
{
	glDetachShader(m_program, m_vertexShader);
	glDeleteShader(m_vertexShader);
	glDetachShader(m_program, m_fragmentShader);
	glDeleteShader(m_fragmentShader);
	glDeleteProgram(m_program);
}

void Shader::enable()
{
	glUseProgram(m_program);
	m_currentShader = this;
}

void Shader::disable()
{
	glUseProgram(0);
	m_currentShader = nullptr;
}

bool Shader::loadShaderSource(const char* filePath, std::string* fileContents)
{
	std::ifstream file;
	char cline[256];

	file.open(filePath);
	if (!file.is_open()) return false;
	while (file.getline(cline, 256)) {
		std::string line(cline);
		*fileContents += line + "\n";
	}
	file.close();
	return true;
}

void Shader::setUniform(const std::string& name, int v)
{
	glUniform1i(glGetUniformLocation(m_program, name.c_str()), v);
}

void Shader::setUniform(const std::string& name, float v)
{
	glUniform1f(glGetUniformLocation(m_program, name.c_str()), v);
}

void Shader::setUniform(const std::string& name, Vec3& v)
{
	glUniform3f(glGetUniformLocation(m_program, name.c_str()), v.getX(), v.getY(), v.getZ());
}

void Shader::setUniform(const std::string& name, Vec2& v)
{
	glUniform2f(glGetUniformLocation(m_program, name.c_str()), v.getX(), v.getY());
}

void Shader::setUniform(const std::string& name, Vec3* v, int nv)
{
	glUniform3fv(glGetUniformLocation(m_program, name.c_str()), nv, (float*)v);
}

void Shader::setUniform(const std::string& name, Vec2* v, int nv)
{
	glUniform2fv(glGetUniformLocation(m_program, name.c_str()), nv, (float*)v);
}

void Shader::setUniform(const std::string& name, Mat4& v)
{
	glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, false, v.getData());
}