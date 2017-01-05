#pragma once

#include "Model.h"

class ModelLoader
{
public:
	static Model* loadModel(const char* filePath, int width, int height, int length, float scale = 1.0f);
private:
	static Model* createModel(unsigned char* data, int width, int height, int length, float scale);
	static unsigned int get(unsigned char* data, int x, int y, int z, int width, int height, int length);
};

