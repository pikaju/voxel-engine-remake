#include "FrustumCuller.h"

#include <glew.h>

FrustumPlane FrustumCuller::planes[6];

void FrustumCuller::extractPlanes(Mat4& matrix)
{
	planes[0] = FrustumPlane(matrix.get(0, 3) + matrix.get(0, 0), matrix.get(1, 3) + matrix.get(1, 0), matrix.get(2, 3) + matrix.get(2, 0), matrix.get(3, 3) + matrix.get(3, 0));
	planes[1] = FrustumPlane(matrix.get(0, 3) - matrix.get(0, 0), matrix.get(1, 3) - matrix.get(1, 0), matrix.get(2, 3) - matrix.get(2, 0), matrix.get(3, 3) - matrix.get(3, 0));
	planes[2] = FrustumPlane(matrix.get(0, 3) + matrix.get(0, 1), matrix.get(1, 3) + matrix.get(1, 1), matrix.get(2, 3) + matrix.get(2, 1), matrix.get(3, 3) + matrix.get(3, 1));
	planes[3] = FrustumPlane(matrix.get(0, 3) - matrix.get(0, 1), matrix.get(1, 3) - matrix.get(1, 1), matrix.get(2, 3) - matrix.get(2, 1), matrix.get(3, 3) - matrix.get(3, 1));
	planes[4] = FrustumPlane(matrix.get(0, 3) + matrix.get(0, 2), matrix.get(1, 3) + matrix.get(1, 2), matrix.get(2, 3) + matrix.get(2, 2), matrix.get(3, 3) + matrix.get(3, 2));
	planes[5] = FrustumPlane(matrix.get(0, 3) - matrix.get(0, 2), matrix.get(1, 3) - matrix.get(1, 2), matrix.get(2, 3) - matrix.get(2, 2), matrix.get(3, 3) - matrix.get(3, 2));
}

bool FrustumCuller::isSphereInFrustum(Vec3& position, float radius)
{
	for (int i = 0; i < 6; i++) {
		if (planes[i].getA() * position.getX() + planes[i].getB() * position.getY() + planes[i].getC() * position.getZ() + planes[i].getD() <= -radius) {
			return false;
		}
	}
	return true;
}