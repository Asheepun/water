#version 330 core

layout (location = 0) in vec3 vertexPosition_attribute;
layout (location = 1) in vec2 textureVertex_attribute;
layout (location = 2) in vec3 vertexNormal_attribute;

out vec2 input_textureCoord;

void main(){

	input_textureCoord = textureVertex_attribute;
	input_textureCoord.y = 1.0 - input_textureCoord.y;

	gl_Position = vec4(vertexPosition_attribute, 1.0);

}

