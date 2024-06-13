#version 330 core
precision mediump float;

in vec2 input_textureCoord;
in vec2 input_localPosition;

out vec4 FragColor;

uniform sampler2D tex;

uniform float widthUnscaled;
uniform float heightUnscaled;

uniform float borderRadius;

void main(){

	vec2 localPosition = vec2(abs(input_localPosition.x) * widthUnscaled - (widthUnscaled - borderRadius * heightUnscaled), abs(input_localPosition.y) * heightUnscaled - (heightUnscaled - borderRadius * heightUnscaled));

	if(localPosition.x > 0.0
	&& localPosition.y > 0.0
	&& length(localPosition) > borderRadius * heightUnscaled){
		discard;
	}

	FragColor = texture(tex, input_textureCoord);

	gl_FragDepth = 0.0;

}
