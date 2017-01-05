#pragma once

#include "Camera.h"

class Mat4
{
public:
	Mat4();
	Mat4(Transform& transform);
	~Mat4();

	void set(Mat4& m);

	void identity();
	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);
	void rotateInverse(float x, float y, float z);
	void scale(float x, float y, float z);

	void projection(float fov, float aspect, float znear, float zfar);
	void ortho(float left = -1.0f, float right = 1.0f, float bottom = 1.0f, float top = -1.0f, float near = 1.0f, float far = -1.0f);

	void camera(Camera& camera);
	void transform(Transform& transform);

	Mat4 operator*(Mat4& v);
	void operator*=(Mat4& v);

	inline void set(int x, int y, float v) { m_v[x + y * 4] = v; }
	inline float get(int x, int y) { return m_v[x + y * 4]; }
	inline float get(int index) { return m_v[index]; }
	inline float* getData() { return m_v; }
private:
	float m_v[16];
};

