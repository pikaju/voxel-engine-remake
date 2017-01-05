#include "ModelLoader.h"

#include <stb_image.h>

#include "VertexData.h"
#include "Util.h"

Model* ModelLoader::loadModel(const char* filePath, int width, int height, int length, float scale)
{
	int x, y, comp;
	unsigned char* pixelData = stbi_load(filePath, &x, &y, &comp, 0);
	Model* model = createModel(pixelData, width, height, length, scale);
	stbi_image_free(pixelData);
	return model;
}

Model* ModelLoader::createModel(unsigned char* data, int width, int height, int length, float scale)
{
	VertexData vertexData;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			for (int z = 0; z < length; z++) {
				Vec3 p1 = Vec3((float)x, (float)y, (float)z) * scale;
				Vec3 p2 = Vec3((float)x + 1.0f, (float)y, (float)z) * scale;
				Vec3 p3 = Vec3((float)x + 1.0f, (float)y, (float)z + 1.0f) * scale;
				Vec3 p4 = Vec3((float)x, (float)y, (float)z + 1.0f) * scale;
				Vec3 p5 = Vec3((float)x, (float)y + 1.0f, (float)z) * scale;
				Vec3 p6 = Vec3((float)x + 1.0f, (float)y + 1.0f, (float)z) * scale;
				Vec3 p7 = Vec3((float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f) * scale;
				Vec3 p8 = Vec3((float)x, (float)y + 1.0f, (float)z + 1.0f) * scale;

				int c = get(data, x, y, z, width, height, length);
				if (c == 0xff00ff) continue;
				Vec3 color = Util::intToColor(c);

				if (get(data, x, y - 1, z, width, height, length) == 0xff00ff) vertexData.quad(p1, p2, p3, p4, color);
				if (get(data, x, y + 1, z, width, height, length) == 0xff00ff) vertexData.quad(p6, p5, p8, p7, color);
				if (get(data, x, y, z - 1, width, height, length) == 0xff00ff) vertexData.quad(p2, p1, p5, p6, color);
				if (get(data, x, y, z + 1, width, height, length) == 0xff00ff) vertexData.quad(p4, p3, p7, p8, color);
				if (get(data, x + 1, y, z, width, height, length) == 0xff00ff) vertexData.quad(p3, p2, p6, p7, color);
				if (get(data, x - 1, y, z, width, height, length) == 0xff00ff) vertexData.quad(p1, p4, p8, p5, color);
			}
		}
	}
	return vertexData.createModel();
}

unsigned int ModelLoader::get(unsigned char* data, int x, int y, int z, int width, int height, int length)
{
	if (x < 0) return 0xff00ff;
	if (y < 0) return 0xff00ff;
	if (z < 0) return 0xff00ff;
	if (x >= width) return 0xff00ff;
	if (y >= height) return 0xff00ff;
	if (z >= length) return 0xff00ff;
	int index = x + z * width + y * width * length;
	return ((unsigned int)data[index * 3 + 0] << 16) | ((unsigned int)data[index * 3 + 1] << 8) | ((unsigned int)data[index * 3 + 2]);
}