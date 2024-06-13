#ifdef __APPLE__

#include "engine/renderer2d.h"

#include "engine/text.h"
#include "engine/files.h"
#include "engine/engine.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "stdio.h"
#include "math.h"
#include "string.h"

struct Arguments{
	Vec4f sizeAndPos;
	Vec4f color;
	Vec4f color2;
	float borderRadius;
	float widthUnscaled;
	float heightUnscaled;
	float alpha;
};

Arguments arguments;

//INIT FUNCTIONS

void Renderer2D_init(Renderer2D_Renderer *renderer_p, int width, int height, AssetManager *assetManager_p){

	renderer_p->offset = getVec2f(0.0, 0.0);
	renderer_p->scale = getVec2f(1.0, 1.0);
	renderer_p->borderRadius = 0.0;

	//renderer_p->width = width;
	//renderer_p->height = height;

	renderer_p->drawAroundCenter = false;

	arguments.alpha = 1.0;

	Model_initFromFile_mesh(&renderer_p->quadModel, Engine_resourcePath / "assets/models/quad.mesh", "quad", Engine_assetManager_p);

	Shader_init(&renderer_p->colorShader, "color", Engine_resourcePath / "shaders/metal/renderer2d/2dshader.metal", "vertexMain", "fragmentMainColor", Engine_assetManager_p);
	Shader_init(&renderer_p->textureShader, "texture", Engine_resourcePath / "shaders/metal/renderer2d/2dshader.metal", "vertexMain", "fragmentMainTexture", Engine_assetManager_p);
	Shader_init(&renderer_p->textureColorShader, "texture-color", Engine_resourcePath / "shaders/metal/renderer2d/2dshader.metal", "vertexMain", "fragmentMainTextureColor", Engine_assetManager_p);
	Shader_init(&renderer_p->iconShader, "icon", Engine_resourcePath / "shaders/metal/renderer2d/2dshader.metal", "vertexMain", "fragmentMainIcon", Engine_assetManager_p);

}

//SETTINGS FUNCTIONS

void Renderer2D_updateRenderCommandEncoder(Renderer2D_Renderer *renderer_p, MTL::RenderCommandEncoder *renderCommandEncoder_p){
	renderer_p->renderCommandEncoder_p = renderCommandEncoder_p;
}

void Renderer2D_updateDrawSize(Renderer2D_Renderer *renderer_p, int width, int height){
	//do nothing
}

void Renderer2D_setDrawAroundCenter(Renderer2D_Renderer *renderer_p, bool flag){
	renderer_p->drawAroundCenter = flag;
}

//DRAWING FUNCTIONS

void Renderer2D_clear(Renderer2D_Renderer *renderer_p){
	//do nothing
}

void Renderer2D_setShader(Renderer2D_Renderer *renderer_p, Shader *shader_p){
	renderer_p->renderCommandEncoder_p->setRenderPipelineState(shader_p->PSO_p);
}

void Renderer2D_setTexture(Renderer2D_Renderer *renderer_p, Texture *texture_p){
	renderer_p->renderCommandEncoder_p->setFragmentTexture(texture_p->pointer, 0);
}

void Renderer2D_setColor(Renderer2D_Renderer *renderer_p, Vec4f color){
	arguments.color = color;
}

void Renderer2D_setColor2(Renderer2D_Renderer *renderer_p, Vec4f color){
	arguments.color2 = color;
}

void Renderer2D_setBorderRadius(Renderer2D_Renderer *renderer_p, float borderRadius){
	arguments.borderRadius = borderRadius;
}

void Renderer2D_setAlpha(Renderer2D_Renderer *renderer_p, float alpha){
	arguments.alpha = alpha;
}

void Renderer2D_setRotation(Renderer2D_Renderer *renderer_p, float rotation){
	//do nothing
}

void Renderer2D_drawRectangle(Renderer2D_Renderer *renderer_p, float x, float y, float width, float height){

	Model *model_p = &renderer_p->quadModel;

	renderer_p->renderCommandEncoder_p->setVertexBuffer(model_p->positionsBuffer_p, 0, 0);
	renderer_p->renderCommandEncoder_p->setVertexBuffer(model_p->textureCoordsBuffer_p, 0, 1);

	arguments.sizeAndPos = getVec4f(
		(2.0 * x + width) / Engine_clientWidth - 1.0,
		1.0 - (2.0 * y + height) / Engine_clientHeight,
		width / Engine_clientWidth,
		height / Engine_clientHeight
	);

	arguments.widthUnscaled = width;
	arguments.heightUnscaled = height;

	renderer_p->renderCommandEncoder_p->setVertexBytes(&arguments, sizeof(Arguments), 2);
	renderer_p->renderCommandEncoder_p->setFragmentBytes(&arguments, sizeof(Arguments), 0);

	renderer_p->renderCommandEncoder_p->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(model_p->n_vertices));

	//print(model_p->n_vertices);

}

void Renderer2D_drawText(Renderer2D_Renderer *renderer_p, const char *text, int x, int y, Font *font_p, int size){

	float scale = (float)size / (float)font_p->size;

	int currentX = 0;

	for(int i = 0; i < strlen(text); i++){

		Glyph *glyph_p = &font_p->glyphs[text[i]];

		Renderer2D_setTexture(renderer_p, &glyph_p->texture);

		Renderer2D_drawRectangle(
			renderer_p,
			x + (float)(currentX + glyph_p->leftSideBearing) * scale,
			y + (float)glyph_p->offsetY * scale,
			(float)glyph_p->width * scale,
			(float)glyph_p->height * scale
		);

		currentX += glyph_p->advanceWidth;

		if(i < strlen(text) - 1){
			currentX += Font_getKern(font_p, text[i], text[i + 1]);
		}
	
	}

}

#endif
