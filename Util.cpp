#include "Util.h"


Vec3 Util::intToColor(int color)
{
	return Vec3(((color >> 16) & 0xff) / 255.0f, ((color >> 8) & 0xff) / 255.0f, ((color >> 0) & 0xff) / 255.0f);
}