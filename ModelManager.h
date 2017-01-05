#pragma once

#include <unordered_map>
#include <string>

#include "Model.h"

class ModelManager
{
public:
	static void load();
	static void cleanup();
	static inline Model* get(const char* path) { return m_models[std::string(path)]; }
private:
	static std::unordered_map<std::string, Model*> m_models;
};

