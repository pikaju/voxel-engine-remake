#version 330

struct Light
{
	vec3 position;
	vec3 color;
};

const int MAX_LIGHTS = 128;

uniform sampler2D pixelPositions;
uniform sampler2D pixelNormals;

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

in vec2 fragmentTexcoord;

layout (location = 0) out vec4 pixelColor;

vec3 calcLight(Light light, vec3 position, vec3 normal);
vec3 calcDirectionalLight(vec3 direction, vec3 color, vec3 normal);

void main()
{
	vec3 fragmentPosition = texture(pixelPositions, fragmentTexcoord).xyz;
	vec3 fragmentNormal = texture(pixelNormals, fragmentTexcoord).xyz;
	
	vec3 totalLight = vec3(0.0);
	for (int i = 0; i < numLights; i++) {
		totalLight += calcLight(lights[i], fragmentPosition, fragmentNormal);
	}
	
	totalLight += calcDirectionalLight(normalize(vec3(-0.5, -1.0, -0.25)), vec3(1.0, 1.0, 1.0), fragmentNormal);
	totalLight += vec3(0.1);
	
	pixelColor = vec4(totalLight, 1.0);
}

vec3 calcLight(Light light, vec3 position, vec3 normal)
{
	vec3 direction = light.position - position;
	float distance = length(direction);
	float angle = clamp(dot(direction / distance, normal), 0.0, 1.0);
	return light.color * angle / ((distance * distance / 128.0) + 1.0);
}

vec3 calcDirectionalLight(vec3 direction, vec3 color, vec3 normal)
{
	float angle = clamp(dot(-direction, normal), -1.0, 1.0) * 0.5 + 0.5;
	return color * angle;
}