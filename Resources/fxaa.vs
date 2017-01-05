#version 130

out vec2 fragmentTexcoord;

void main()
{
	fragmentTexcoord = gl_Vertex.xy * 0.5 + 0.5;
	gl_Position = gl_Vertex;
}