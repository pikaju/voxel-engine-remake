#version 330

uniform sampler2D pixelColors;
uniform sampler2D pixelPositions;
uniform sampler2D pixelLighting;
uniform sampler2D pixelSSAO;
uniform sampler2D pixelSky;

uniform float fogDistance;
uniform vec3 cameraPosition;

uniform sampler2D pixelShadow;
uniform mat4 shadowMapMVP;
uniform vec2 shadowMapTexelSize;

in vec2 fragmentTexcoord;

layout (location = 0) out vec4 fragColor;

float getShadow(sampler2D map, vec2 coordinates, float compare);
float getShadowLinear(sampler2D map, vec2 coordinates, float compare);

void main()
{
	vec3 lightingColor = texture(pixelLighting, fragmentTexcoord).rgb;
	vec4 surfaceColor = texture(pixelColors, fragmentTexcoord);
	float ssao = 0.0;
	vec2 texelSize = vec2(1.0) / vec2(1280.0, 720.0);
	float count = 0.0;
	float blurAmount = 1.0;
	for (float x = fragmentTexcoord.x - texelSize.x * blurAmount; x <= fragmentTexcoord.x + texelSize.x * blurAmount; x += texelSize.x) {
		for (float y = fragmentTexcoord.y - texelSize.y * blurAmount; y <= fragmentTexcoord.y + texelSize.y * blurAmount; y += texelSize.y) {
			ssao += texture(pixelSSAO, vec2(x, y)).r;
			count++;
		}
	}
	ssao /= count;
	vec3 color = surfaceColor.rgb * ssao;
	color *= lightingColor;
	vec3 vertexPosition = texture(pixelPositions, fragmentTexcoord).xyz;
	float distanceToCamera = length(vertexPosition - cameraPosition);
	
	float fog = distanceToCamera / fogDistance * surfaceColor.a;
	fog *= fog;
	fragColor = vec4(color, surfaceColor.a);
	fragColor = mix(fragColor, vec4(0.0), clamp(fog, 0.0, 1.0));
	fragColor = mix(fragColor, texture(pixelSky, fragmentTexcoord), 1.0 - fragColor.a);
	//fragColor = vec4(texture(pixelSSAO, fragmentTexcoord).rgb, 1.0);
}

float getShadow(sampler2D map, vec2 coordinates, float compare)
{
	return compare > texture(map, coordinates).r ? 1.0 : 0.0;
}

float getShadowLinear(sampler2D map, vec2 coordinates, float compare)
{
	vec2 integerCoordinates = coordinates / shadowMapTexelSize + vec2(0.5);
	vec2 fractCoordinates = fract(integerCoordinates);
	vec2 startCoordinates = (integerCoordinates - fractCoordinates) * shadowMapTexelSize;
	
	float bl = getShadow(map, startCoordinates, compare);
	float br = getShadow(map, startCoordinates + vec2(shadowMapTexelSize.x, 0.0), compare);
	float tl = getShadow(map, startCoordinates + vec2(0.0, shadowMapTexelSize.y), compare);
	float tr = getShadow(map, startCoordinates + shadowMapTexelSize, compare);
	
	return mix(mix(bl, br, fractCoordinates.x), mix(tl, tr, fractCoordinates.x), fractCoordinates.y);
}