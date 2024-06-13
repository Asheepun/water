#version 330 core
precision mediump float;

in vec2 input_textureCoord;
in vec2 input_localPosition;

out vec4 FragColor;

uniform sampler2D tex;

uniform float widthUnscaled;
uniform float heightUnscaled;

uniform float borderRadius;

uniform float pixelWidth;
uniform float pixelHeight;

void main(){

	FragColor = vec4(0.0);
	//for(int x = -2; x < 3; x++){
		//for(int y = -2; y < 3; y++){
			//FragColor += texture(tex, input_textureCoord + vec2(pixelWidth * float(x), pixelHeight * float(y)));
		//}
	//}
	/*
	int r = 1;
	for(int x = 1 -r; x < r; x++){
		for(int y = 1 -r; y < r; y++){
			FragColor += texture(tex, input_textureCoord + vec2(pixelWidth * x, pixelHeight * y)) * float(x * x + y * y < 6);
		}
	}
	*/

	FragColor += texture(tex, input_textureCoord + vec2(0.0, 0.0));
	FragColor += texture(tex, input_textureCoord + vec2(pixelWidth * 0.5, 0.0));
	FragColor += texture(tex, input_textureCoord + vec2(-pixelWidth * 0.5, 0.0));
	FragColor += texture(tex, input_textureCoord + vec2(0.0, pixelHeight * 0.5));
	FragColor += texture(tex, input_textureCoord + vec2(0.0, -pixelHeight * 0.5));

	FragColor /= FragColor.w;
	FragColor.w *= 0.9;
	//FragColor.w = 1.0;
	//FragColor /= 9.0;
	//FragColor.w = 1.0;
	//FragColor = texture(tex, input_textureCoord + vec2(pixelWidth, pixelHeight));
	//FragColor = vec4(0.0, 1.0, 0.0, 1.0);

	gl_FragDepth = 0.0;

}

