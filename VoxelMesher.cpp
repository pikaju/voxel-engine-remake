#include "VoxelMesher.h"

#include "World.h"
#include "Util.h"

#include <chrono>
#include <iostream>

static int triangleTable[256][16] =
{ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
{ 8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1 },
{ 3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1 },
{ 4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
{ 4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1 },
{ 9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1 },
{ 10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1 },
{ 5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
{ 5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1 },
{ 8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1 },
{ 2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
{ 2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1 },
{ 11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1 },
{ 5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1 },
{ 11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1 },
{ 11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1 },
{ 2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1 },
{ 6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
{ 3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1 },
{ 6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
{ 6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1 },
{ 8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1 },
{ 7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1 },
{ 3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
{ 0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1 },
{ 9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1 },
{ 8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
{ 5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1 },
{ 0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1 },
{ 6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1 },
{ 10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
{ 1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1 },
{ 0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1 },
{ 3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
{ 6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1 },
{ 9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1 },
{ 8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1 },
{ 3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1 },
{ 10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1 },
{ 10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
{ 2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1 },
{ 7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
{ 2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1 },
{ 1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1 },
{ 11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1 },
{ 8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1 },
{ 0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1 },
{ 7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1 },
{ 7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1 },
{ 10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1 },
{ 0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1 },
{ 7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1 },
{ 6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1 },
{ 4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1 },
{ 10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1 },
{ 8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1 },
{ 1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1 },
{ 10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1 },
{ 10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1 },
{ 9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1 },
{ 7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1 },
{ 3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1 },
{ 7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1 },
{ 3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1 },
{ 6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1 },
{ 9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1 },
{ 1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1 },
{ 4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1 },
{ 7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1 },
{ 6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1 },
{ 0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1 },
{ 6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1 },
{ 0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1 },
{ 11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1 },
{ 6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1 },
{ 5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1 },
{ 9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1 },
{ 1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1 },
{ 10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1 },
{ 0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1 },
{ 11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1 },
{ 9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1 },
{ 7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1 },
{ 2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1 },
{ 9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1 },
{ 9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1 },
{ 1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1 },
{ 0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1 },
{ 10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1 },
{ 2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1 },
{ 0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1 },
{ 0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1 },
{ 9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1 },
{ 5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1 },
{ 5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1 },
{ 8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1 },
{ 9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1 },
{ 1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1 },
{ 3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1 },
{ 4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1 },
{ 9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1 },
{ 11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1 },
{ 2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1 },
{ 9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1 },
{ 3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1 },
{ 1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1 },
{ 4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1 },
{ 0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1 },
{ 1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 } };

#define ANTI_VOXEL_BLEEDING 0.001f
#define OPTIMIZE_QUADS 1

void VoxelMesher::meshChunk(VertexData& data, Chunk& chunk)
{
	std::vector<Quad> quads;
	unsigned int blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	chunk.getBlocks(blocks);
	int cx = chunk.getX() * CHUNK_SIZE;
	int cy = chunk.getY() * CHUNK_SIZE;
	int cz = chunk.getZ() * CHUNK_SIZE;
	// Top
	for (int l = 0; l < CHUNK_SIZE; l++) {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				int xx = i;
				int yy = l;
				int zz = j;
				int x = xx + cx;
				int y = yy + cy;
				int z = zz + cz;
				Color color(blocks[xx + yy * CHUNK_SIZE + zz * CHUNK_SIZE * CHUNK_SIZE]);
				if (color.m_a == 0) continue;

				bool isOpaque = false;
				if (yy < CHUNK_SIZE - 1) {
					isOpaque = blocks[xx + (yy + 1) * CHUNK_SIZE + zz * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000;
				} else {
					isOpaque = chunk.getNeighbor(UP)->getBlock(xx, 0, zz) > 0xff000000;
				}
				if (!isOpaque) quads.emplace_back(Vec3((float)x, (float)y + 1.0f, (float)z), Vec3((float)x, (float)y + 1.0f, (float)z + 1.0f), Vec3((float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f), Vec3((float)x + 1.0f, (float)y + 1.0f, (float)z), color); // Finds all visible quads
			}
		}
#if OPTIMIZE_QUADS == 1
		optimizeQuads(quads);
#endif
		for (unsigned int i = 0; i < quads.size(); i++) {
			quads[i].increaseSize(ANTI_VOXEL_BLEEDING);
			data.quad(quads[i].getP0(), quads[i].getP1(), quads[i].getP2(), quads[i].getP3(), quads[i].getColor()); // Adds quads to mesh
		}
		quads.clear();
	}
	// Bottom
	for (int l = 0; l < CHUNK_SIZE; l++) {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				int xx = i;
				int yy = l;
				int zz = j;
				int x = xx + cx;
				int y = yy + cy;
				int z = zz + cz;
				Color color(blocks[xx + yy * CHUNK_SIZE + zz * CHUNK_SIZE * CHUNK_SIZE]);
				if (color.m_a == 0) continue;

				bool isOpaque = false;
				if (yy > 0) {
					isOpaque = blocks[xx + (yy - 1) * CHUNK_SIZE + zz * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000;
				} else {
					isOpaque = chunk.getNeighbor(DOWN)->getBlock(xx, CHUNK_SIZE - 1, zz) > 0xff000000;
				}
				if (!isOpaque) quads.emplace_back(Vec3((float)x, (float)y, (float)z), Vec3((float)x + 1.0f, (float)y, (float)z), Vec3((float)x + 1.0f, (float)y, (float)z + 1.0f), Vec3((float)x, (float)y, (float)z + 1.0f), color); // Finds all visible quads
			}
		}
#if OPTIMIZE_QUADS == 1
		optimizeQuads(quads);
#endif
		for (unsigned int i = 0; i < quads.size(); i++) {
			data.quad(quads[i].getP0(), quads[i].getP1(), quads[i].getP2(), quads[i].getP3(), quads[i].getColor()); // Adds quads to mesh
		}
		quads.clear();
	}
	// Left
	for (int l = 0; l < CHUNK_SIZE; l++) {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				int xx = l;
				int yy = i;
				int zz = j;
				int x = xx + cx;
				int y = yy + cy;
				int z = zz + cz;
				Color color(blocks[xx + yy * CHUNK_SIZE + zz * CHUNK_SIZE * CHUNK_SIZE]);
				if (color.m_a == 0) continue;

				bool isOpaque = false;
				if (xx > 0) {
					isOpaque = blocks[(xx - 1) + yy * CHUNK_SIZE + zz * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000;
				} else {
					isOpaque = chunk.getNeighbor(LEFT)->getBlock(CHUNK_SIZE - 1, yy, zz) > 0xff000000;
				}
				if (!isOpaque) quads.emplace_back(Vec3((float)x, (float)y, (float)z), Vec3((float)x, (float)y, (float)z + 1.0f), Vec3((float)x, (float)y + 1.0f, (float)z + 1), Vec3((float)x, (float)y + 1.0f, (float)z), color); // Finds all visible quads
			}
		}
#if OPTIMIZE_QUADS == 1
		optimizeQuads(quads);
#endif
		for (unsigned int i = 0; i < quads.size(); i++) {
			data.quad(quads[i].getP0(), quads[i].getP1(), quads[i].getP2(), quads[i].getP3(), quads[i].getColor()); // Adds quads to mesh
		}
		quads.clear();
	}
	// Right
	for (int l = 0; l < CHUNK_SIZE; l++) {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				int xx = l;
				int yy = i;
				int zz = j;
				int x = xx + cx;
				int y = yy + cy;
				int z = zz + cz;
				Color color(blocks[xx + yy * CHUNK_SIZE + zz * CHUNK_SIZE * CHUNK_SIZE]);
				if (color.m_a == 0) continue;

				bool isOpaque = false;
				if (xx < CHUNK_SIZE - 1) {
					isOpaque = blocks[(xx + 1) + yy * CHUNK_SIZE + zz * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000;
				} else {
					isOpaque = chunk.getNeighbor(RIGHT)->getBlock(0, yy, zz) > 0xff000000;
				}
				if (!isOpaque) quads.emplace_back(Vec3((float)x + 1.0f, (float)y, (float)z), Vec3((float)x + 1.0f, (float)y + 1.0f, (float)z), Vec3((float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f), Vec3((float)x + 1.0f, (float)y, (float)z + 1.0f), color); // Finds all visible quads
			}
		}
#if OPTIMIZE_QUADS == 1
		optimizeQuads(quads);
#endif
		for (unsigned int i = 0; i < quads.size(); i++) {
			data.quad(quads[i].getP0(), quads[i].getP1(), quads[i].getP2(), quads[i].getP3(), quads[i].getColor()); // Adds quads to mesh
		}
		quads.clear();
	}
	// Front
	for (int l = 0; l < CHUNK_SIZE; l++) {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				int xx = i;
				int yy = j;
				int zz = l;
				int x = xx + cx;
				int y = yy + cy;
				int z = zz + cz;
				Color color(blocks[xx + yy * CHUNK_SIZE + zz * CHUNK_SIZE * CHUNK_SIZE]);
				if (color.m_a == 0) continue;

				bool isOpaque = false;
				if (zz < CHUNK_SIZE - 1) {
					isOpaque = blocks[xx + yy * CHUNK_SIZE + (zz + 1) * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000;
				} else {
					isOpaque = chunk.getNeighbor(FRONT)->getBlock(xx, yy, 0) > 0xff000000;
				}
				if (!isOpaque) quads.emplace_back(Vec3((float)x, (float)y, (float)z + 1.0f), Vec3((float)x + 1, (float)y, (float)z + 1.0f), Vec3((float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f), Vec3((float)x, (float)y + 1.0f, (float)z + 1.0f), color); // Finds all visible quads
			}
		}
#if OPTIMIZE_QUADS == 1
		optimizeQuads(quads);
#endif
		for (unsigned int i = 0; i < quads.size(); i++) {
			data.quad(quads[i].getP0(), quads[i].getP1(), quads[i].getP2(), quads[i].getP3(), quads[i].getColor()); // Adds quads to mesh
		}
		quads.clear();
	}
	// Back
	for (int l = 0; l < CHUNK_SIZE; l++) {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				int xx = i;
				int yy = j;
				int zz = l;
				int x = xx + cx;
				int y = yy + cy;
				int z = zz + cz;
				Color color(blocks[xx + yy * CHUNK_SIZE + zz * CHUNK_SIZE * CHUNK_SIZE]);
				if (color.m_a == 0) continue;

				bool isOpaque = false;
				if (zz > 0) {
					isOpaque = blocks[xx + yy * CHUNK_SIZE + (zz - 1) * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000;
				} else {
					isOpaque = chunk.getNeighbor(BACK)->getBlock(xx, yy, CHUNK_SIZE - 1) > 0xff000000;
				}
				if (!isOpaque) quads.emplace_back(Vec3((float)x, (float)y, (float)z), Vec3((float)x, (float)y + 1.0f, (float)z), Vec3((float)x + 1.0f, (float)y + 1.0f, (float)z), Vec3((float)x + 1.0f, (float)y, (float)z), color); // Finds all visible quads
			}
		}
#if OPTIMIZE_QUADS == 1
		optimizeQuads(quads);
#endif
		for (unsigned int i = 0; i < quads.size(); i++) {
			data.quad(quads[i].getP0(), quads[i].getP1(), quads[i].getP2(), quads[i].getP3(), quads[i].getColor()); // Adds quads to mesh
		}
		quads.clear();
	}
}

void VoxelMesher::meshChunkMarchingCubes(VertexData& data, Chunk& chunk)
{
	unsigned int blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	chunk.getBlocks(blocks);
	ChunkManager& chunkManager = chunk.getWorld()->getChunkManager();
	int cx = chunk.getX() * CHUNK_SIZE;
	int cy = chunk.getY() * CHUNK_SIZE;
	int cz = chunk.getZ() * CHUNK_SIZE;
	Vec3 points[12] = {
		Vec3((float)cx + 0.5f, (float)cy, (float)cz),
		Vec3((float)cx + 1.0f, (float)cy, (float)cz + 0.5f),
		Vec3((float)cx + 0.5f, (float)cy, (float)cz + 1.0f),
		Vec3((float)cx, (float)cy, (float)cz + 0.5f),

		Vec3((float)cx + 0.5f, (float)cy + 1.0f, (float)cz),
		Vec3((float)cx + 1.0f, (float)cy + 1.0f, (float)cz + 0.5f),
		Vec3((float)cx + 0.5f, (float)cy + 1.0f, (float)cz + 1.0f),
		Vec3((float)cx, (float)cy + 1.0f, (float)cz + 0.5f),

		Vec3((float)cx, (float)cy + 0.5f, (float)cz),
		Vec3((float)cx + 1.0f, (float)cy + 0.5f, (float)cz),
		Vec3((float)cx + 1.0f, (float)cy + 0.5f, (float)cz + 1.0f),
		Vec3((float)cx, (float)cy + 0.5f, (float)cz + 1.0f),
	};
	std::vector<Vec3> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int x = 0; x < CHUNK_SIZE; x++) {
		for (unsigned int y = 0; y < CHUNK_SIZE; y++) {
			for (unsigned int z = 0; z < CHUNK_SIZE; z++) {
				int triangleIndex = 0;
				if (x == CHUNK_SIZE - 1 || y == CHUNK_SIZE - 1 || z == CHUNK_SIZE - 1) {
					//if (chunkManager.isSolid(cx + x, cy + y, cz + z)) triangleIndex |= 1;
					//if (chunkManager.isSolid(cx + x + 1, cy + y, cz + z)) triangleIndex |= 2;
					//if (chunkManager.isSolid(cx + x + 1, cy + y, cz + z + 1)) triangleIndex |= 4;
					//if (chunkManager.isSolid(cx + x, cy + y, cz + z + 1)) triangleIndex |= 8;
					//if (chunkManager.isSolid(cx + x, cy + y + 1, cz + z)) triangleIndex |= 16;
					//if (chunkManager.isSolid(cx + x + 1, cy + y + 1, cz + z)) triangleIndex |= 32;
					//if (chunkManager.isSolid(cx + x + 1, cy + y + 1, cz + z + 1)) triangleIndex |= 64;
					//if (chunkManager.isSolid(cx + x, cy + y + 1, cz + z + 1)) triangleIndex |= 128;
				} else {
					if (blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000) triangleIndex |= 1;
					if (blocks[(x + 1) + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000) triangleIndex |= 2;
					if (blocks[(x + 1) + y * CHUNK_SIZE + (z + 1) * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000) triangleIndex |= 4;
					if (blocks[x + y * CHUNK_SIZE + (z + 1) * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000) triangleIndex |= 8;
					if (blocks[x + (y + 1) * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000) triangleIndex |= 16;
					if (blocks[(x + 1) + (y + 1) * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000) triangleIndex |= 32;
					if (blocks[(x + 1) + (y + 1) * CHUNK_SIZE + (z + 1) * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000) triangleIndex |= 64;
					if (blocks[x + (y + 1) * CHUNK_SIZE + (z + 1) * CHUNK_SIZE * CHUNK_SIZE] > 0xff000000) triangleIndex |= 128;
				}
				
				for (unsigned int i = 0; triangleTable[triangleIndex][i] != -1; i++) {
					Vec3 point = points[triangleTable[triangleIndex][i]] + Vec3((float)x + 0.5f, (float)y + 0.5f, (float)z + 0.5f);
					bool foundMatch = false;
					if (!foundMatch) {
						indices.push_back(vertices.size());
						vertices.push_back(point);
					}
					//data.triangle(points[triangleTable[triangleIndex][i]] + Vec3((float)x + 0.5f, (float)y + 0.5f, (float)z + 0.5f), points[triangleTable[triangleIndex][i + 1]] + Vec3((float)x + 0.5f, (float)y + 0.5f, (float)z + 0.5f), points[triangleTable[triangleIndex][i + 2]] + Vec3((float)x + 0.5f, (float)y + 0.5f, (float)z + 0.5f), Vec3(0.5f));
				}
				unsigned int indexOffset = data.getNumVertices();
				for (unsigned int i = 0; i < vertices.size(); i++) data.addVertex(Vertex(vertices[i], Color(Vec3(0.5f)), Vec3()));
				for (unsigned int i = 0; i < indices.size(); i++) data.addIndex(indices[i] + indexOffset);
				vertices.clear();
				indices.clear();
			}
		}
	}
	data.calculateNormals();
}

void VoxelMesher::optimizeQuads(std::vector<Quad>& quads)
{
	for (unsigned int i = 0; i < quads.size(); i++) {
		for (unsigned int j = i + 1; j < quads.size(); j++) {
			if (!(quads[i].getColor() == quads[j].getColor())) continue;
			Vec3 a0[4];
			a0[0] = quads[i].getP0();
			a0[1] = quads[i].getP1();
			a0[2] = quads[i].getP2();
			a0[3] = quads[i].getP3();

			Vec3 a1[4];
			a1[0] = quads[j].getP0();
			a1[1] = quads[j].getP1();
			a1[2] = quads[j].getP2();
			a1[3] = quads[j].getP3();

			int matches = countMatches(a0, a1);

			if (matches < 2) continue;

			quads[j] = Quad(quads[i], quads[j]);
			quads[i] = quads.back();
			quads.pop_back();
			
			i = i - 1;
			break;
		}
	}
}

int VoxelMesher::countMatches(Vec3* v0, Vec3* v1)
{
	int matches = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (v0[i] == v1[j]) {
				matches++;
			}
		}
	}
	return matches;
}