#version 330

uniform vec3 sunDirection;

in vec3 vertexPosition;

layout (location = 0) out vec4 fragColor;

void main()
{
	vec3 skyPosition = normalize(vertexPosition);
	float height = skyPosition.y;
	vec3 horizon = vec3(1.0 + (-height * 0.25));
	vec3 skyColor = vec3(0.2, 0.5, 1.0) * horizon;
	float sunAngle = clamp(dot(normalize(vertexPosition), normalize(sunDirection)), 0.0, 1.0) - 0.3;
	vec3 sun = vec3(sunAngle * sunAngle * sunAngle);
	//skyColor = vec3(0.025, 0.05, 0.1) * horizon;
	fragColor = vec4(skyColor + sun, 1.0);
}