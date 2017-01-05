#version 330

in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec3 fragmentNormal;

layout (location = 0) out vec4 pixelColor;
layout (location = 1) out vec4 pixelPosition;
layout (location = 2) out vec4 pixelNormal;

void main()
{
	pixelColor = fragmentColor;
	pixelPosition = vec4(fragmentPosition, 1.0);
	pixelNormal = vec4(fragmentNormal, 1.0);
}