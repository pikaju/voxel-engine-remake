#version 130

out vec3 vertexPosition;

uniform mat4 mvpMatrix;

void main()
{
	vertexPosition = gl_Vertex.xyz;
	gl_Position = gl_Vertex * mvpMatrix;
}