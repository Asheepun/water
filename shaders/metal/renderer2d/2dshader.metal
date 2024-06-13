#include <metal_stdlib>
using namespace metal;

struct Arguments{
	float4 sizeAndPos;
	float4 color;
	float4 color2;
	float borderRadius;
	float widthUnscaled;
	float heightUnscaled;
	float alpha;
};

struct v2f{
	float4 position [[position]];
	float2 textureCoord;
	float2 localPosition;
};

v2f vertex vertexMain(
	uint vertexID [[vertex_id]],
	device const float4 *positions [[buffer(0)]],
	device const float2 *textureCoords [[buffer(1)]],
	constant Arguments& arguments[[buffer(2)]]
){

	float4 position = float4(positions[vertexID].xyz, 1.0);
	float2 textureCoord = textureCoords[vertexID].xy;

	float2 pos = arguments.sizeAndPos.xy;
	float2 size = arguments.sizeAndPos.zw;

	position.xy *= size;
	position.xy += pos;

	v2f output;
	output.position = position;
	output.textureCoord = textureCoord;
	output.localPosition = positions[vertexID].xy;

	return output;

}

half4 fragment fragmentMainColor(
	v2f input [[stage_in]],
	constant Arguments& arguments[[buffer(0)]]
){

	half4 outputColor = half4(arguments.color);

	outputColor.w *= half(arguments.alpha);

	float2 localPosition = float2(
		abs(input.localPosition.x) * arguments.widthUnscaled - (arguments.widthUnscaled - arguments.borderRadius * arguments.heightUnscaled),
		abs(input.localPosition.y) * arguments.heightUnscaled - (arguments.heightUnscaled - arguments.borderRadius * arguments.heightUnscaled)
	);

	if(localPosition.x > 0.0
	&& localPosition.y > 0.0
	&& length(localPosition) > arguments.borderRadius * arguments.heightUnscaled){
		outputColor.w = 0.0;
	}

	return outputColor;

}

half4 fragment fragmentMainTexture(
	v2f input [[stage_in]],
	constant Arguments& arguments[[buffer(0)]],
	texture2d<half, access::sample> colorTexture [[texture(0)]]
){

	constexpr sampler s(address::repeat, filter::linear);

	half4 outputColor = colorTexture.sample(s, float2(input.textureCoord.x, input.textureCoord.y));

	outputColor.w *= half(arguments.alpha);

	return outputColor;

}

half4 fragment fragmentMainTextureColor(
	v2f input [[stage_in]],
	constant Arguments& arguments[[buffer(0)]],
	texture2d<half, access::sample> colorTexture [[texture(0)]]
){

	constexpr sampler s(address::repeat, filter::linear);

	half4 outputColor = colorTexture.sample(s, float2(input.textureCoord.x, input.textureCoord.y));

	outputColor.w *= half(arguments.alpha);
	
	outputColor *= half4(arguments.color);

	return outputColor;

}

half4 fragment fragmentMainIcon(
	v2f input [[stage_in]],
	constant Arguments& arguments[[buffer(0)]],
	texture2d<half, access::sample> colorTexture [[texture(0)]]
){

	constexpr sampler s(address::repeat, filter::linear);

	half4 outputColor = colorTexture.sample(s, float2(input.textureCoord.x, input.textureCoord.y));

	outputColor = outputColor.r * half4(arguments.color) + outputColor.g * half4(arguments.color2);

	outputColor.w *= half(arguments.alpha);

	float2 localPosition = float2(
		abs(input.localPosition.x) * arguments.widthUnscaled - (arguments.widthUnscaled - arguments.borderRadius * arguments.heightUnscaled),
		abs(input.localPosition.y) * arguments.heightUnscaled - (arguments.heightUnscaled - arguments.borderRadius * arguments.heightUnscaled)
	);

	if(localPosition.x > 0.0
	&& localPosition.y > 0.0
	&& length(localPosition) > arguments.borderRadius * arguments.heightUnscaled){
		outputColor.w = 0.0;
	}

	return outputColor;

}
