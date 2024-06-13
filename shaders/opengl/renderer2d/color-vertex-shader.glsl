#version 330 core
layout (location = 0) in vec2 vertexPosition_attribute;
layout (location = 1) in vec2 textureVertex_attribute;

out vec2 input_textureCoord;
out vec2 input_localPosition;

uniform float posX;
uniform float posY;

uniform float width;
uniform float height;

vec2 vertexPosition;

uniform float aspectRatio;

void main(){

	input_textureCoord = textureVertex_attribute;

	input_localPosition = vertexPosition_attribute;

	vertexPosition = vertexPosition_attribute;

	vertexPosition.x *= width;
	vertexPosition.y *= height;

	gl_Position = vec4(
		vertexPosition.x / aspectRatio - 1.0 + posX / aspectRatio,
		vertexPosition.y + 1.0 - posY,
		0.0,
		1.0
	);

}
