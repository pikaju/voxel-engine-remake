#version 330

uniform sampler2D pixelColors;
uniform vec2 texelSize;

in vec2 fragmentTexcoord;

layout (location = 0) out vec4 pixelColor;

void main()
{
	vec3 rgbNW = texture(pixelColors, fragmentTexcoord + vec2(-texelSize.x, -texelSize.y)).xyz;
	vec3 rgbNE = texture(pixelColors, fragmentTexcoord + vec2(texelSize.x, -texelSize.y)).xyz;
	vec3 rgbSW = texture(pixelColors, fragmentTexcoord + vec2(-texelSize.x, texelSize.y)).xyz;
	vec3 rgbSE = texture(pixelColors, fragmentTexcoord + vec2(texelSize.x, texelSize.y)).xyz;
	vec3 rgb = texture(pixelColors, fragmentTexcoord).xyz;
	
	vec3 lumaVector = vec3(0.299, 0.587, 0.114);
	float lumaNW = dot(rgbNW, lumaVector);
	float lumaNE = dot(rgbNE, lumaVector);
	float lumaSW = dot(rgbSW, lumaVector);
	float lumaSE = dot(rgbSE, lumaVector);
	float luma = dot(rgb, lumaVector);
	
	const float FXAA_REDUCE_MIN = 1.0 / 128.0;
	const float FXAA_REDUCE_MUL = 1.0 / 8.0;
	const float FXAA_SPAN_MAX = 8.0;
	
	vec2 dir;
	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
	dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));
	
	float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (FXAA_REDUCE_MUL * 0.25), FXAA_REDUCE_MIN);
	float inverseDirAdjustment = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
	
	dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), 
		max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * inverseDirAdjustment));
	
	dir.x = dir.x * step(1.0, abs(dir.x));
	dir.y = dir.y * step(1.0, abs(dir.y));
	
	dir = dir * texelSize;

	vec3 result1 = (1.0/2.0) * (
		texture(pixelColors, fragmentTexcoord + (dir * vec2(1.0/3.0 - 0.5))).xyz +
		texture(pixelColors, fragmentTexcoord + (dir * vec2(2.0/3.0 - 0.5))).xyz);

	vec3 result2 = result1 * (1.0/2.0) + (1.0/4.0) * (
		texture(pixelColors, fragmentTexcoord + (dir * vec2(0.0/3.0 - 0.5))).xyz +
		texture(pixelColors, fragmentTexcoord + (dir * vec2(3.0/3.0 - 0.5))).xyz);

	float lumaMin = min(luma, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
	float lumaMax = max(luma, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
	float lumaResult2 = dot(result2, lumaVector);
	
	if(lumaResult2 < lumaMin || lumaResult2 > lumaMax)
		pixelColor = vec4(result1, 1.0);
	else
		pixelColor = vec4(result2, 1.0);
		
	//pixelColor = vec4(rgb, 1.0);
}