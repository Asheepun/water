#version 330 core
precision mediump float;

in vec2 input_textureCoord;

out vec4 FragColor;

uniform sampler2D tex;
uniform sampler2D windowTexture;

uniform float pixelWidth;
uniform float pixelHeight;

uniform vec4 windowColor;

void main(){

	if(texture(windowTexture, input_textureCoord).x == 0.0){
		FragColor = windowColor;
		gl_FragDepth = 0.0;
		return;
	}

	vec4 inputColor = texture(tex, input_textureCoord);

	FragColor = inputColor;

	float checkGroundTolerance = 0.5;
	float groundTolerance = 0.3;

	if(inputColor.x < checkGroundTolerance
	&& inputColor.y < checkGroundTolerance
	&& inputColor.z < checkGroundTolerance){

		vec4 average = vec4(0.0);
		for(int y = -2; y < 3; y++){
			for(int x = -2; x < 3; x++){
				average += texture(tex, input_textureCoord + vec2(float(x) * pixelWidth, float(y) * pixelHeight));
			}
		}

		//average /= 49.0;
		average /= 25.0;

		if(average.x < groundTolerance
		&& average.y < groundTolerance
		&& average.z < groundTolerance){
			FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		}else if(inputColor.x < groundTolerance
		&& inputColor.y < groundTolerance
		&& inputColor.z < groundTolerance){
			FragColor = average;
		}

	}

	gl_FragDepth = 0.0;

}
