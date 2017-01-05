#version 330

const float numSamples = 16.0;

uniform sampler2D pixelPositions;
uniform vec2 texelSize;
uniform vec3 cameraPosition;
uniform vec2 sampleKernel[int(numSamples)];

in vec2 fragmentTexcoord;

layout (location = 0) out vec4 fragColor;

float getDistanceToCamera(vec2 texcoord);

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	vec3 vertexPosition = texture(pixelPositions, fragmentTexcoord).xyz;
	float distanceToCamera = getDistanceToCamera(fragmentTexcoord);
	
	float avarage = 0.0;
	float iterations = 0.0;
	float rotation = rand(fragmentTexcoord) * 3.1415 * 2.0;
	float s = sin(rotation);
	float c = cos(rotation);
	for (int i = 0; i < int(numSamples); i++) {
		vec2 scale = vec2(1.0 / (distanceToCamera / 2.5)) * normalize(texelSize);
		vec2 sk = sampleKernel[i];
		vec2 rsk = vec2(sk.x * c - sk.y * s, sk.y * c + sk.x * s);
		float difference = getDistanceToCamera(rsk * scale + fragmentTexcoord) - distanceToCamera;
		avarage += clamp(difference, -0.25, 0.0);
		iterations++;
	}

	avarage /= iterations;
	fragColor = vec4(avarage * 8.0 + 1.0);
}

float getDistanceToCamera(vec2 texcoord) {
	return length(texture(pixelPositions, texcoord).xyz - (cameraPosition));
}