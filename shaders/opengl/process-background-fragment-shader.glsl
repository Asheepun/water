#version 330 core
precision mediump float;

in vec2 input_textureCoord;

out vec4 FragColor;

uniform sampler2D tex;
uniform sampler2D windowTexture;

uniform float pixelWidth;
uniform float pixelHeight;

uniform vec4 windowColor;

vec3 RGBToHSL(vec3 rgb){
	
	float minChannel = min(min(rgb.x, rgb.y), rgb.z);
	float maxChannel = max(max(rgb.x, rgb.y), rgb.z);

	float luminance = (minChannel + maxChannel) / 2.0;

	float saturation;
	if(luminance <= 0.5){
		saturation = (maxChannel - minChannel) / (maxChannel + minChannel);
	}else{
		saturation = (maxChannel - minChannel) / (2.0 - maxChannel - minChannel);
	}

	if(abs(minChannel - maxChannel) < 0.01){
		return vec3(0.0, saturation, luminance);
	}

	float hue = 0.0;
	if(abs(maxChannel - rgb.x) < 0.01){
		hue = (rgb.y - rgb.z) / (maxChannel - minChannel);
	}
	if(abs(maxChannel - rgb.y) < 0.01){
		hue = 2.0 + (rgb.z - rgb.x) / (maxChannel - minChannel);
	}
	if(abs(maxChannel - rgb.z) < 0.01){
		hue = 4.0 + (rgb.x - rgb.y) / (maxChannel - minChannel);
	}

	hue *= 60.0;
	if(hue < 0.0){
		hue += 360.0;
	}

	return vec3(hue, saturation, luminance);

}

vec3 RGBToHSV(vec3 rgb){
	
	float minChannel = min(min(rgb.x, rgb.y), rgb.z);
	float maxChannel = max(max(rgb.x, rgb.y), rgb.z);

	float value = maxChannel;

	float saturation = 0.0;
	if(maxChannel > 0.0){
		saturation = (maxChannel - minChannel) / maxChannel;
	}

	if(abs(minChannel - maxChannel) < 0.001){
		return vec3(0.0, saturation, value);
	}

	float hue = 0.0;
	if(abs(maxChannel - rgb.x) < 0.01){
		hue = (rgb.y - rgb.z) / (maxChannel - minChannel);
	}
	if(abs(maxChannel - rgb.y) < 0.01){
		hue = 2.0 + (rgb.z - rgb.x) / (maxChannel - minChannel);
	}
	if(abs(maxChannel - rgb.z) < 0.01){
		hue = 4.0 + (rgb.x - rgb.y) / (maxChannel - minChannel);
	}

	hue *= 60.0;
	if(hue < 0.0){
		hue += 360.0;
	}

	return vec3(hue, saturation, value);


}

void main(){

	if(texture(windowTexture, input_textureCoord).x == 0.0){
		FragColor = windowColor;
		gl_FragDepth = 0.0;
		return;
	}

	vec4 inputColor = texture(tex, input_textureCoord);

	//vec3 HSL = RGBToHSL(inputColor.xyz);
	vec3 HSV = RGBToHSV(inputColor.xyz);

	float checkGroundTolerance = 0.5;
	float groundTolerance = 0.3;

	FragColor = inputColor;

	if(HSV.x > 181.0 && HSV.x < 300.0
	&& HSV.y > 0.3){
		FragColor = inputColor;
		FragColor.w = 0.5;
	}

	/*
	if(abs(HSL.x - 210.0) < 60.0
	&& HSL.y > 0.15
	&& HSL.z < 0.95){
		FragColor = inputColor;
		FragColor.w = 0.5;
	}
	*/

	if(inputColor.x > 0.0
	|| inputColor.y > 0.0
	|| inputColor.z > 0.0){
		for(int y = -1; y < 2; y++){
			for(int x = -1; x < 2; x++){
				vec4 samplePixel = texture(tex, input_textureCoord + vec2(float(x) * pixelWidth, float(y) * pixelHeight));
				if(samplePixel.x == 0.0
				&& samplePixel.y == 0.0
				&& samplePixel.z == 0.0){
					FragColor = vec4(0.0, 0.0, 0.0, 1.0);
				}
			}
		}
	}

	gl_FragDepth = 0.0;

}
