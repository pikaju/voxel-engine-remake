#pragma once

#include "Window.h"

class RenderingEngine
{
public:
	static void init();
	static void cleanup();

	static void setWireframe(bool wireframe);
	static inline void setWireframe() { setWireframe(m_wireframe); }
	static void toggleWireframe();
private:
	static bool m_wireframe;
};

