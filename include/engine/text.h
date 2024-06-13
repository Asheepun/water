typedef struct Glyph{

	Texture texture;

	int width;
	int height;
	int offsetX;
	int offsetY;

	int advanceWidth;
	int leftSideBearing;

	bool inited;

	UnicodeCharacter character;

}Glyph;

struct GFont{

	Array<Glyph> glyphs;
	//Glyph glyphs[255];

	int size;

	char *fileData;
	stbtt_fontinfo info;

	int ascent;
	int descent;
	int lineGap;

	float scale;

	//char name[255];

};
