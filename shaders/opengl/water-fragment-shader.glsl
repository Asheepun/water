#version 330 core
precision mediump float;

in vec2 input_textureCoord;
in vec2 input_localPosition;

out vec4 FragColor;

uniform sampler2D tex;

uniform float widthUnscaled;
uniform float heightUnscaled;

uniform float borderRadius;

uniform vec2 textureScale;

void main(){

	FragColor = texture(tex, input_textureCoord);

	//vec4 extraColor = texture(tex, input_textureCoord + vec2(-textureScale.x, -textureScale.y))
					//+ texture(tex, input_textureCoord + vec2(textureScale.x, -textureScale.y))
					//+ texture(tex, input_textureCoord + vec2(-textureScale.x, textureScale.y))
					//+ texture(tex, input_textureCoord + vec2(textureScale.x, textureScale.y));
	
	//FragColor += extraColor * float(extraColor.w > 2.0);

	FragColor.w /= min(FragColor.w, 0.001);

	gl_FragDepth = 0.0;

}

