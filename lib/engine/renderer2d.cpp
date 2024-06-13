//#include "engine/text.h"
#include "engine/renderer2d.h"
//#include "engine/shaders.h"
//#include "engine/3d.h"
//#include "engine/files.h"
//#include "engine/shaders.h"
//#include "engine/engine.h"

//#include "stdio.h"
//#include "math.h"
//#include "string.h"

//INIT FUNCTIONS

void Renderer2D_init(Renderer2D_Renderer *renderer_p, int width, int height, AssetManager *assetManager_p){

	renderer_p->offset = getVec2f(0.0, 0.0);
	renderer_p->scale = getVec2f(1.0, 1.0);
	renderer_p->borderRadius = 0.0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//renderer_p->width = width;
	//renderer_p->height = height;

	renderer_p->drawAroundCenter = false;

	static float rectangleVertices[] = {

		1, 1, 		1, 0,
		1, -1, 		1, 1,
		-1, -1, 	0, 1,

		1, 1, 		1, 0,
		-1, -1, 	0, 1,
		-1, 1, 		0, 0,

	};

	glGenBuffers(1, &renderer_p->rectangleVBO);

	glBindBuffer(GL_ARRAY_BUFFER, renderer_p->rectangleVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &renderer_p->rectangleVAO);
	glBindVertexArray(renderer_p->rectangleVAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Font font = getFont("assets/fonts/times.ttf", 100);

	//Texture_initFromText(&renderer_p->textTexture, "", font);

	Shader_init(&renderer_p->colorShader, 		 "color", "shaders/opengl/renderer2d/color-vertex-shader.glsl", "shaders/opengl/renderer2d/color-fragment-shader.glsl", assetManager_p);
	Shader_init(&renderer_p->textureShader, 	 "color", "shaders/opengl/renderer2d/color-vertex-shader.glsl", "shaders/opengl/renderer2d/texture-fragment-shader.glsl", assetManager_p);
	Shader_init(&renderer_p->textureColorShader, "color", "shaders/opengl/renderer2d/color-vertex-shader.glsl", "shaders/opengl/renderer2d/texture-color-fragment-shader.glsl", assetManager_p);
	Shader_init(&renderer_p->iconShader, 		 "color", "shaders/opengl/renderer2d/color-vertex-shader.glsl", "shaders/opengl/renderer2d/icon-fragment-shader.glsl", assetManager_p);

}

void Renderer2D_free(Renderer2D_Renderer *renderer_p){

	glDeleteBuffers(1, &renderer_p->rectangleVBO);
	glDeleteVertexArrays(1, &renderer_p->rectangleVAO);

	Shader_free(&renderer_p->colorShader);
	Shader_free(&renderer_p->textureShader);
	Shader_free(&renderer_p->textureColorShader);
	Shader_free(&renderer_p->iconShader);

}

//SETTINGS FUNCTIONS

void Renderer2D_updateDrawSize(Renderer2D_Renderer *renderer_p, int width, int height){

	/*
	float newWidth = width;
	float newHeight = height;

	float offsetX = 0;
	float offsetY = 0;

	float aspectRatio = (float)renderer_p->width / (float)renderer_p->height;

	if(newWidth / newHeight > aspectRatio){
		newWidth = height * aspectRatio;
	}else{
		newHeight = width / aspectRatio;
	}

	offsetX = (width - newWidth) / 2;
	offsetY = (height - newHeight) / 2;

	glViewport((int)offsetX, (int)offsetY, (int)newWidth, (int)newHeight);
	*/

}

void Renderer2D_setDrawAroundCenter(Renderer2D_Renderer *renderer_p, bool flag){
	renderer_p->drawAroundCenter = flag;
}

//DRAWING FUNCTIONS

void Renderer2D_clear(Renderer2D_Renderer *renderer_p){

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

}

void Renderer2D_setShader(Renderer2D_Renderer *renderer_p, Shader *shader){
	renderer_p->currentShader = *shader;
	glUseProgram(renderer_p->currentShader.ID);
}

void Renderer2D_setTexture(Renderer2D_Renderer *renderer_p, Texture *texture_p){
	GL3D_uniformTexture(renderer_p->currentShader.ID, "tex", 0, texture_p->ID);
}

void Renderer2D_setColor(Renderer2D_Renderer *renderer_p, Vec4f color){
	GL3D_uniformVec4f(renderer_p->currentShader.ID, "color", color);
}

void Renderer2D_setColor2(Renderer2D_Renderer *renderer_p, Vec4f color){
	GL3D_uniformVec4f(renderer_p->currentShader.ID, "backgroundColor", color);
}

void Renderer2D_setBorderRadius(Renderer2D_Renderer *renderer_p, float borderRadius){
	renderer_p->borderRadius = borderRadius;
}

void Renderer2D_setAlpha(Renderer2D_Renderer *renderer_p, float alpha){
	GL3D_uniformFloat(renderer_p->currentShader.ID, "alpha", alpha);
}

void Renderer2D_setRotation(Renderer2D_Renderer *renderer_p, float rotation){
	Mat2f rotationMatrix = getRotationMat2f(rotation);
	GL3D_uniformMat2f(renderer_p->currentShader.ID, "rotationMatrix", rotationMatrix);
}

void Renderer2D_drawRectangle(Renderer2D_Renderer *renderer_p, float x, float y, float width, float height){

	glBindBuffer(GL_ARRAY_BUFFER, renderer_p->rectangleVBO);
	glBindVertexArray(renderer_p->rectangleVAO);

	if(!renderer_p->drawAroundCenter){
		x += width / 2.0;
		y += height / 2.0;
	}

	GL3D_uniformFloat(renderer_p->currentShader.ID, "posX", renderer_p->scale.x * 2.0 * ((float)x + renderer_p->offset.x) / (float)Engine_clientWidth);
	GL3D_uniformFloat(renderer_p->currentShader.ID, "posY", renderer_p->scale.y * 2.0 * ((float)y + renderer_p->offset.y) / (float)Engine_clientHeight);
	GL3D_uniformFloat(renderer_p->currentShader.ID, "width", renderer_p->scale.x * (float)width / (float)Engine_clientWidth);
	GL3D_uniformFloat(renderer_p->currentShader.ID, "height", renderer_p->scale.y * (float)height / (float)Engine_clientHeight);

	GL3D_uniformFloat(renderer_p->currentShader.ID, "widthUnscaled", renderer_p->scale.x * width);
	GL3D_uniformFloat(renderer_p->currentShader.ID, "heightUnscaled", renderer_p->scale.y * height);

	GL3D_uniformFloat(renderer_p->currentShader.ID, "aspectRatio", 1.0);

	GL3D_uniformFloat(renderer_p->currentShader.ID, "borderRadius", renderer_p->borderRadius);

	/*
	GL3D_uniformFloat(renderer_p->currentShader, "posX", 2 * ((float)x + renderer_p->offset.x) / (float)renderer_p->height);
	GL3D_uniformFloat(renderer_p->currentShader, "posY", 2 * ((float)y + renderer_p->offset.y) / (float)renderer_p->height);
	GL3D_uniformFloat(renderer_p->currentShader, "width", (float)width / (float)renderer_p->height);
	GL3D_uniformFloat(renderer_p->currentShader, "height", (float)height / (float)renderer_p->height);

	GL3D_uniformFloat(renderer_p->currentShader, "aspectRatio", (float)renderer_p->width / (float)renderer_p->height);
	*/

	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Renderer2D_drawText(Renderer2D_Renderer *renderer_p, const char *text, int x, int y, GFont *font_p, int size){

	Array<UnicodeCharacter> characters = getUnicodeCharacters_mustFree(text);

	float scale = (float)size / (float)font_p->size;

	int currentX = 0;

	for(int i = 0; i < characters.length; i++){

		Glyph *glyph_p = GFont_glyph(font_p, characters[i]);

		Renderer2D_setTexture(renderer_p, &glyph_p->texture);

		Renderer2D_drawRectangle(
			renderer_p,
			x + (float)(currentX + glyph_p->leftSideBearing) * scale,
			y + (float)(font_p->ascent + glyph_p->offsetY) * scale,
			(float)glyph_p->width * scale,
			(float)glyph_p->height * scale
		);

		currentX += glyph_p->advanceWidth;

		/*
		if(i < strlen(text) - 1){
			currentX += GFont_getKern(font_p, text[i], text[i + 1]);
		}
		*/
	
	}

	characters.freeMemory();

}
