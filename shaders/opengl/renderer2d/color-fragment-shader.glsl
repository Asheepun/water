#version 330 core
precision mediump float;

in vec2 input_localPosition;

out vec4 FragColor;

uniform vec4 color;

uniform float widthUnscaled;
uniform float heightUnscaled;

uniform float borderRadius;

uniform float alpha;

void main(){

	vec2 localPosition = vec2(abs(input_localPosition.x) * widthUnscaled - (widthUnscaled - borderRadius * heightUnscaled), abs(input_localPosition.y) * heightUnscaled - (heightUnscaled - borderRadius * heightUnscaled));

	if(localPosition.x > 0.0
	&& localPosition.y > 0.0
	&& length(localPosition) > borderRadius * heightUnscaled){
		discard;
	}

	FragColor = color;
	FragColor.w *= alpha;

	//FragColor = vec4(widthUnscaled / heightUnscaled, 0.0, 0.0, 1.0);

	gl_FragDepth = 0.0;

}
