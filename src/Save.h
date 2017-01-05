#pragma once

#include <fstream>

#include "DataContainer.h"

class Save
{
public:
	static void openOutput(const char* file);
	static void writeDataContainer(DataContainer& container);
	static void closeOutput();
private:
	static std::ofstream output;
	static std::ifstream input;
};

