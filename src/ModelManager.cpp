#include "ModelManager.h"

#include "ModelLoader.h"

std::unordered_map<std::string, Model*> ModelManager::m_models;

struct ModelData
{
	ModelData(std::string path, int width, int height, int length, float scale) : m_path(path), m_width(width), m_height(height), m_length(length), m_scale(scale) {  }
	std::string m_path;
	int m_width;
	int m_height;
	int m_length;
	float m_scale;
};

void ModelManager::load()
{
	const int numFiles = 4;
	ModelData files[numFiles] = {
		ModelData("Resources/Models/Human/head.png", 16, 16, 16, 0.125f),
		ModelData("Resources/Models/Human/body.png", 8, 8, 8, 0.125f),
		ModelData("Resources/Models/Human/foot.png", 8, 8, 8, 0.125f),
		ModelData("Resources/Models/Human/hand.png", 4, 4, 4, 0.125f),
	};

	for (unsigned int i = 0; i < numFiles; i++) {
		if (m_models.find(files[i].m_path) != m_models.end()) {
			delete m_models[files[i].m_path];
		}
		m_models[files[i].m_path] = ModelLoader::loadModel(files[i].m_path.c_str(), files[i].m_width, files[i].m_height, files[i].m_length, files[i].m_scale);
	}
}

void ModelManager::cleanup()
{

}