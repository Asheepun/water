#include "engine/text.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

GFont getFont(const char *fontPath, int fontSize, AssetManager *assetManager_p){

	GFont font;

	font.glyphs.init();

	font.size = fontSize;

	long int fileSize;
	font.fileData = getAssetData_mustFree(fontPath, &fileSize, assetManager_p);

	if(!stbtt_InitFont(&font.info, (unsigned char *)font.fileData, 0)){
		printf("!!********************!!\n");
		printf("Could not init font: %s\n", fontPath);
		printf("!!********************!!\n");
	}

	stbtt_GetFontVMetrics(&font.info, &font.ascent, &font.descent, &font.lineGap);

	font.scale = stbtt_ScaleForPixelHeight(&font.info, font.size);

	font.ascent = roundf(font.ascent * font.scale);
	font.descent = roundf(font.descent * font.scale);
	font.lineGap = roundf(font.lineGap * font.scale);

	return font;
	
}

void GFont_free(GFont *font_p){
	for(int i = 0; i < font_p->glyphs.length; i++){
		Texture_free(&font_p->glyphs[i].texture);
	}
	free(font_p->fileData);
}

Glyph *GFont_glyph(GFont *font_p, UnicodeCharacter character){

	for(int i = 0; i < font_p->glyphs.length; i++){
		if(font_p->glyphs[i].character == character){
			return &font_p->glyphs[i];
		}
	}

	Glyph glyph;
	glyph.character = character;

	stbtt_GetCodepointHMetrics(&font_p->info, getCodePoint(character), &glyph.advanceWidth, &glyph.leftSideBearing);

	glyph.advanceWidth *= font_p->scale;
	glyph.leftSideBearing *= font_p->scale;

	unsigned char *bitMap = stbtt_GetCodepointBitmap(
		&font_p->info,
		font_p->scale,
		font_p->scale,
		getCodePoint(character),
		&glyph.width,
		&glyph.height,
		&glyph.offsetX,
		&glyph.offsetY
	);

	unsigned char *rgbaMap = (unsigned char *)malloc(glyph.width * glyph.height * 4 * sizeof(unsigned char));

	for(int i = 0; i < glyph.width * glyph.height; i++){
		rgbaMap[i * 4 + 0] = bitMap[i];
		rgbaMap[i * 4 + 1] = bitMap[i];
		rgbaMap[i * 4 + 2] = bitMap[i];
		rgbaMap[i * 4 + 3] = bitMap[i];
	}

	Texture_init(&glyph.texture, "glyph", rgbaMap, glyph.width, glyph.height);

	free(bitMap);
	free(rgbaMap);

	font_p->glyphs.push(glyph);

	return &font_p->glyphs[font_p->glyphs.length - 1];

}

int GFont_getKern(GFont *font_p, char character1, char character2){

	return 0;

	/*
	int kern = stbtt_GetCodepointKernAdvance(&font_p->info, character1, character2);

	return kern;
	*/
	
}

int GFont_getTextWidth(GFont *font_p, const char *text, int size){

	Array<UnicodeCharacter> characters = getUnicodeCharacters_mustFree(text);

	int width = 0;

	for(int i = 0; i < characters.length; i++){

		Glyph *glyph_p = GFont_glyph(font_p, characters[i]);

		width += glyph_p->advanceWidth;

		/*
		if(i < strlen(text) - 1){
			width += GFont_getKern(font_p, text[i], text[i + 1]);
		}
		*/

	}

	characters.freeMemory();

	return width * (float)size / (float)font_p->size;

}
