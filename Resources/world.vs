#version 330

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec3 vertexNormal;

out vec3 fragmentPosition;
out vec4 fragmentColor;
out vec3 fragmentNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	fragmentPosition = (vec4(vertexPosition, 1.0) * modelMatrix).xyz;
	fragmentColor = vertexColor;
	fragmentNormal = (vec4(vertexNormal, 0.0) * modelMatrix).xyz;
	gl_Position =  vec4(vertexPosition, 1.0) * modelMatrix * viewMatrix * projectionMatrix;
}