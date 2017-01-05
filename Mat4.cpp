#include "Mat4.h"

#include <math.h>

Mat4::Mat4()
{
	identity();
}

Mat4::Mat4(Transform& transformation)
{
	transform(transformation);
}

Mat4::~Mat4()
{
}

void Mat4::set(Mat4& m)
{
	for (int i = 0; i < 16; i++) {
		m_v[i] = m.m_v[i];
	}
}

void Mat4::identity()
{
	set(0, 0, 1.0f); set(1, 0, 0.0f); set(2, 0, 0.0f); set(3, 0, 0.0f);
	set(0, 1, 0.0f); set(1, 1, 1.0f); set(2, 1, 0.0f); set(3, 1, 0.0f);
	set(0, 2, 0.0f); set(1, 2, 0.0f); set(2, 2, 1.0f); set(3, 2, 0.0f);
	set(0, 3, 0.0f); set(1, 3, 0.0f); set(2, 3, 0.0f); set(3, 3, 1.0f);
}

void Mat4::translate(float x, float y, float z)
{
	set(0, 0, 1.0f); set(1, 0, 0.0f); set(2, 0, 0.0f); set(3, 0, x);
	set(0, 1, 0.0f); set(1, 1, 1.0f); set(2, 1, 0.0f); set(3, 1, y);
	set(0, 2, 0.0f); set(1, 2, 0.0f); set(2, 2, 1.0f); set(3, 2, z);
	set(0, 3, 0.0f); set(1, 3, 0.0f); set(2, 3, 0.0f); set(3, 3, 1.0f);
}

void Mat4::rotate(float x, float y, float z)
{
	Mat4 xMatrix;
	Mat4 yMatrix;
	Mat4 zMatrix;

	if (x != 0) {
		float sin = sinf(x * ((float)3.14159265358979323846 / 180.0f));
		float cos = cosf(x * ((float)3.14159265358979323846 / 180.0f));
		xMatrix.set(0, 0, 1); xMatrix.set(1, 0, 0); xMatrix.set(2, 0, 0); xMatrix.set(3, 0, 0);
		xMatrix.set(0, 1, 0); xMatrix.set(1, 1, cos); xMatrix.set(2, 1, -sin); xMatrix.set(3, 1, 0);
		xMatrix.set(0, 2, 0); xMatrix.set(1, 2, sin); xMatrix.set(2, 2, cos); xMatrix.set(3, 2, 0);
		xMatrix.set(0, 3, 0); xMatrix.set(1, 3, 0); xMatrix.set(2, 3, 0); xMatrix.set(3, 3, 1);
	} else {
		xMatrix.identity();
	}
	if (y != 0) {
		float sin = sinf(y * ((float)3.14159265358979323846 / 180.0f));
		float cos = cosf(y * ((float)3.14159265358979323846 / 180.0f));
		yMatrix.set(0, 0, cos); yMatrix.set(1, 0, 0); yMatrix.set(2, 0, sin); yMatrix.set(3, 0, 0);
		yMatrix.set(0, 1, 0); yMatrix.set(1, 1, 1); yMatrix.set(2, 1, 0); yMatrix.set(3, 1, 0);
		yMatrix.set(0, 2, -sin); yMatrix.set(1, 2, 0); yMatrix.set(2, 2, cos); yMatrix.set(3, 2, 0);
		yMatrix.set(0, 3, 0); yMatrix.set(1, 3, 0); yMatrix.set(2, 3, 0); yMatrix.set(3, 3, 1);
	} else {
		yMatrix.identity();
	}
	if (z != 0) {
		float sin = sinf(z * ((float)3.14159265358979323846 / 180.0f));
		float cos = cosf(z * ((float)3.14159265358979323846 / 180.0f));
		zMatrix.set(0, 0, cos); zMatrix.set(1, 0, -sin); zMatrix.set(2, 0, 0); zMatrix.set(3, 0, 0);
		zMatrix.set(0, 1, sin); zMatrix.set(1, 1, cos); zMatrix.set(2, 1, 0); zMatrix.set(3, 1, 0);
		zMatrix.set(0, 2, 0); zMatrix.set(1, 2, 0); zMatrix.set(2, 2, 1); zMatrix.set(3, 2, 0);
		zMatrix.set(0, 3, 0); zMatrix.set(1, 3, 0); zMatrix.set(2, 3, 0); zMatrix.set(3, 3, 1);
	} else {
		zMatrix.identity();
	}

	Mat4 finalMatrix = xMatrix * yMatrix * zMatrix;
	for (int i = 0; i < 16; i++) { m_v[i] = finalMatrix.m_v[i]; }
}

void Mat4::rotateInverse(float x, float y, float z)
{
	Mat4 xMatrix;
	Mat4 yMatrix;
	Mat4 zMatrix;

	if (x != 0) {
		float sin = sinf(x * ((float)3.14159265358979323846 / 180.0f));
		float cos = cosf(x * ((float)3.14159265358979323846 / 180.0f));
		xMatrix.set(0, 0, 1); xMatrix.set(1, 0, 0); xMatrix.set(2, 0, 0); xMatrix.set(3, 0, 0);
		xMatrix.set(0, 1, 0); xMatrix.set(1, 1, cos); xMatrix.set(2, 1, -sin); xMatrix.set(3, 1, 0);
		xMatrix.set(0, 2, 0); xMatrix.set(1, 2, sin); xMatrix.set(2, 2, cos); xMatrix.set(3, 2, 0);
		xMatrix.set(0, 3, 0); xMatrix.set(1, 3, 0); xMatrix.set(2, 3, 0); xMatrix.set(3, 3, 1);
	} else {
		xMatrix.identity();
	}
	if (y != 0) {
		float sin = sinf(y * ((float)3.14159265358979323846 / 180.0f));
		float cos = cosf(y * ((float)3.14159265358979323846 / 180.0f));
		yMatrix.set(0, 0, cos); yMatrix.set(1, 0, 0); yMatrix.set(2, 0, sin); yMatrix.set(3, 0, 0);
		yMatrix.set(0, 1, 0); yMatrix.set(1, 1, 1); yMatrix.set(2, 1, 0); yMatrix.set(3, 1, 0);
		yMatrix.set(0, 2, -sin); yMatrix.set(1, 2, 0); yMatrix.set(2, 2, cos); yMatrix.set(3, 2, 0);
		yMatrix.set(0, 3, 0); yMatrix.set(1, 3, 0); yMatrix.set(2, 3, 0); yMatrix.set(3, 3, 1);
	} else {
		yMatrix.identity();
	}
	if (z != 0) {
		float sin = sinf(z * ((float)3.14159265358979323846 / 180.0f));
		float cos = cosf(z * ((float)3.14159265358979323846 / 180.0f));
		zMatrix.set(0, 0, cos); zMatrix.set(1, 0, -sin); zMatrix.set(2, 0, 0); zMatrix.set(3, 0, 0);
		zMatrix.set(0, 1, sin); zMatrix.set(1, 1, cos); zMatrix.set(2, 1, 0); zMatrix.set(3, 1, 0);
		zMatrix.set(0, 2, 0); zMatrix.set(1, 2, 0); zMatrix.set(2, 2, 1); zMatrix.set(3, 2, 0);
		zMatrix.set(0, 3, 0); zMatrix.set(1, 3, 0); zMatrix.set(2, 3, 0); zMatrix.set(3, 3, 1);
	} else {
		zMatrix.identity();
	}

	Mat4 finalMatrix = zMatrix * yMatrix * xMatrix;
	for (int i = 0; i < 16; i++) { m_v[i] = finalMatrix.m_v[i]; }
}

void Mat4::scale(float x, float y, float z)
{
	set(0, 0, x); set(1, 0, 0.0f); set(2, 0, 0.0f); set(3, 0, 0.0f);
	set(0, 1, 0.0f); set(1, 1, y); set(2, 1, 0.0f); set(3, 1, 0.0f);
	set(0, 2, 0.0f); set(1, 2, 0.0f); set(2, 2, z); set(3, 2, 0.0f);
	set(0, 3, 0.0f); set(1, 3, 0.0f); set(2, 3, 0.0f); set(3, 3, 1.0f);
}

void Mat4::camera(Camera& camera)
{
	Transform transform = camera.getTransform();
	
	Mat4 translation;
	Mat4 rotation;
	Mat4 scale;

	translation.translate(-transform.getTranslation().getX(), -transform.getTranslation().getY(), -transform.getTranslation().getZ());
	rotation.rotateInverse(transform.getRotation().getX(), transform.getRotation().getY(), transform.getRotation().getZ());
	scale.scale(transform.getScale().getX(), transform.getScale().getY(), transform.getScale().getZ());

	Mat4 finalMatrix = scale * translation * rotation;
	for (int i = 0; i < 16; i++) m_v[i] = finalMatrix.m_v[i];
}

void Mat4::transform(Transform& transform)
{
	Mat4 translation;
	Mat4 rotation;
	Mat4 scale;

	translation.translate(transform.getTranslation().getX(), transform.getTranslation().getY(), transform.getTranslation().getZ());
	rotation.rotate(-transform.getRotation().getX(), -transform.getRotation().getY(), -transform.getRotation().getZ());
	scale.scale(transform.getScale().getX(), transform.getScale().getY(), transform.getScale().getZ());

	Mat4 finalMatrix = rotation * translation * scale;
	for (int i = 0; i < 16; i++) m_v[i] = finalMatrix.m_v[i];
}

void Mat4::projection(float fov, float aspect, float znear, float zfar)
{
	float tanHalfFOV = tanf((fov * 0.5f) * ((float)3.14159265358979323846 / 180.0f));

	set(0, 0, 1.0f / (tanHalfFOV * aspect)); set(1, 0, 0); set(2, 0, 0); set(3, 0, 0);
	set(0, 1, 0); set(1, 1, 1.0f / tanHalfFOV); set(2, 1, 0); set(3, 1, 0);
	set(0, 2, 0); set(1, 2, 0); set(2, 2, -(zfar + znear) / (zfar - znear)); set(3, 2, -(2 * zfar * znear) / (zfar - znear));
	set(0, 3, 0); set(1, 3, 0); set(2, 3, -1); set(3, 3, 0);
}

void Mat4::ortho(float left, float right, float bottom, float top, float near, float far)
{
	set(0, 0, 2.0f / (right - left)); set(1, 0, 0); set(2, 0, 0); set(3, 0, -((right + left) / (right - left)));
	set(0, 1, 0); set(1, 1, 2.0f / (top - bottom)); set(2, 1, 0); set(3, 1, -((top + bottom) / (top - bottom)));
	set(0, 2, 0); set(1, 2, 0); set(2, 2, -2.0f / (far - near)); set(3, 2, -((far + near) / (far - near)));
	set(0, 3, 0); set(1, 3, 0); set(2, 3, 0); set(3, 3, 1);
}

Mat4 Mat4::operator*(Mat4& v)
{
	Mat4 m;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			m.set(x, y, get(x, 0) * v.get(0, y) + get(x, 1) * v.get(1, y) + get(x, 2) * v.get(2, y) + get(x, 3) * v.get(3, y));
		}
	}
	return m;
}

void Mat4::operator*=(Mat4& v)
{
	Mat4 m = operator*(v);
	set(m);
}