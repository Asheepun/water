#version 330 core
precision mediump float;

in vec2 input_localPosition;
in vec2 input_textureCoord;

out vec4 FragColor;

uniform sampler2D tex;

uniform vec4 color;
uniform vec4 backgroundColor;

uniform float widthUnscaled;
uniform float heightUnscaled;

uniform float alpha;

uniform float borderRadius;

void main(){

	vec2 localPosition = vec2(abs(input_localPosition.x) * widthUnscaled - (widthUnscaled - borderRadius * heightUnscaled), abs(input_localPosition.y) * heightUnscaled - (heightUnscaled - borderRadius * heightUnscaled));

	if(localPosition.x > 0.0
	&& localPosition.y > 0.0
	&& length(localPosition) > borderRadius * heightUnscaled){
		discard;
	}

	vec4 textureColor = texture(tex, input_textureCoord);

	FragColor = textureColor.r * color + textureColor.g * backgroundColor;
	FragColor.w = textureColor.w * alpha;

	//if(gl_FragColor.w < 1.0){
		//discard;
	//}

	gl_FragDepth = 0.0;

}

