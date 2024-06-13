#version 330 core
precision mediump float;

precision mediump float;

in vec2 input_textureCoord;

out vec4 FragColor;

uniform sampler2D tex;

uniform vec4 color;

void main(){

	float tolerance = 0.0001;

	if(input_textureCoord.x > 1.0 - tolerance
	|| input_textureCoord.x < tolerance 
	|| input_textureCoord.y > 1.0 - tolerance
	|| input_textureCoord.y < tolerance){
		discard;
	}

	FragColor = color;
	FragColor.w *= texture(tex, input_textureCoord).w;

	//if(gl_FragColor.w < 1.0){
		//discard;
	//}

	gl_FragDepth = 0.0;

}

