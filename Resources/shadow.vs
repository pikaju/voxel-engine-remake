#version 130

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = gl_Vertex * modelMatrix * viewMatrix * projectionMatrix;
}