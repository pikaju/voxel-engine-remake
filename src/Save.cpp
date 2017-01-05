#include "Save.h"

std::ofstream Save::output;
std::ifstream Save::input;

void Save::openOutput(const char* file)
{
	output.open(file);
}

void Save::writeDataContainer(DataContainer& container)
{
	output.write(container.getData(), container.getSize());
}

void Save::closeOutput()
{
	output.close();
}