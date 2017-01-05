#version 330

uniform sampler2D pixelColors;
uniform vec2 texelSize;

in vec2 fragmentTexcoord;

layout (location = 0) out vec4 pixelColor;

void main()
{
	vec4 normalColor = texture(pixelColors, fragmentTexcoord);
	const float radius = 2.0;
	const float scale = 2.0;
	vec3 blurColor = vec3(0.0);
	float iterations = 0.0;
	for (float x = -texelSize.x * radius; x <= texelSize.x * radius; x += texelSize.x) {
		for (float y = -texelSize.y * radius; y <= texelSize.y * radius; y += texelSize.y) {
			blurColor += texture(pixelColors, fragmentTexcoord + vec2(x, y) * scale).xyz;
			iterations++;
		}
	}
	blurColor /= iterations;
	blurColor = clamp(blurColor.xyz * blurColor.xyz - 0.1, 0.0, 1.0);
	
	pixelColor = vec4(normalColor.rgb + blurColor, normalColor.a);
}