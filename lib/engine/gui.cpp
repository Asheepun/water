#include "engine/gui.h"
//#include "engine/engine.h"

//#include "stdlib.h"
//#include "string.h"
//#include "math.h"

enum ElementType{
	ELEMENT_TYPE_RECT,
	ELEMENT_TYPE_TEXT,
	ELEMENT_TYPE_TEXTURE,
	ELEMENT_TYPE_ICON,
	ELEMENT_TYPE_COLOR_WHEEL,
	ELEMENT_TYPE_COLOR_TRIANGLE,
};

struct Element{
	enum ElementType type;
	int x;
	int y;
	int width;
	int height;
	float borderRadius;
	unsigned int shader;
	Vec4f color;
	Texture texture;
	GFont *font_p;
	String text;
};

Shader colorWheelShader;
Shader colorTriangleShader;

//std::vector<Element> elements;
Array<Element> elements;

//int WIDTH = 1920;
//int HEIGHT = 1080;

//int GUI_activeID = -1;
SmallString GUI_activeTag;
bool GUI_isHovered = false;
bool GUI_textInputIsActive = false;

SmallString GUI_keyboard_activeTag;
SmallString GUI_keyboard_nextActiveTag;

Array<SmallString> GUI_keyboard_tagList;
//std::vector<SmallString> GUI_keyboard_tagList;
bool GUI_keyboard_lastShouldBecomeActive = false;
bool GUI_keyboard_nextShouldBecomeActive = false;

Engine_Key actionKey;
Engine_Key upKey;
Engine_Key downKey;
Engine_Key leftKey;
Engine_Key rightKey;

int upKeyCounter = 0;
int downKeyCounter = 0;
int leftKeyCounter = 0;
int rightKeyCounter = 0;

float GUI_borderRadius = 0.0;
Vec4f GUI_color = getVec4f(0.3, 0.3, 0.3, 1.0);
Vec4f GUI_backgroundColor = getVec4f(0.7, 0.7, 0.7, 1.0);
Vec2f GUI_scale = getVec2f(1.0, 1.0);

void GUI_init(){

	elements.init();
	GUI_keyboard_tagList.init();

#ifndef __APPLE__
	//Shader_init(&colorWheelShader, "color-wheel-shader", "shaders/opengl/renderer2d/color-vertex-shader.glsl", "shaders/opengl/color-wheel-fragment-shader.glsl", Engine_assetManager_p);
	//Shader_init(&colorTriangleShader, "color-triangle-shader", "shaders/opengl/renderer2d/color-vertex-shader.glsl", "shaders/opengl/color-triangle-fragment-shader.glsl", Engine_assetManager_p);
#endif

}

void GUI_free(){
	Shader_free(&colorWheelShader);
	Shader_free(&colorTriangleShader);
}

void GUI_reset(){

	elements.clear();

	GUI_isHovered = false;
	GUI_borderRadius = 0.0;

}

void GUI_keyboard_reset(Engine_Key input_actionKey, Engine_Key input_upKey, Engine_Key input_downKey, Engine_Key input_leftKey, Engine_Key input_rightKey){

	actionKey = input_actionKey;
	upKey = input_upKey;
	downKey = input_downKey;
	leftKey = input_leftKey;
	rightKey = input_rightKey;

	if(upKey.downed){
		upKeyCounter = 0;
	}
	if(downKey.downed){
		downKeyCounter = 0;
	}
	if(leftKey.downed){
		leftKeyCounter = 0;
	}
	if(rightKey.downed){
		rightKeyCounter = 0;
	}

	int startTime = 8;
	int repeatTime = 8;

	if(upKey.down && upKeyCounter >= startTime && upKeyCounter % repeatTime == 0){
		upKey.downed = true;
	}
	if(downKey.down && downKeyCounter >= startTime && downKeyCounter % repeatTime == 0){
		downKey.downed = true;
	}
	if(leftKey.down && leftKeyCounter >= startTime && leftKeyCounter % repeatTime == 0){
		leftKey.downed = true;
	}
	if(rightKey.down && rightKeyCounter >= startTime && rightKeyCounter % repeatTime == 0){
		rightKey.downed = true;
	}

	upKeyCounter++;
	downKeyCounter++;
	leftKeyCounter++;
	rightKeyCounter++;

	if(GUI_keyboard_lastShouldBecomeActive){
		GUI_keyboard_nextActiveTag = GUI_keyboard_tagList[GUI_keyboard_tagList.length - 1];
		GUI_keyboard_lastShouldBecomeActive = false;
	}

	GUI_keyboard_activeTag = GUI_keyboard_nextActiveTag;

	GUI_keyboard_tagList.clear();

}

void GUI_draw(Renderer2D_Renderer *renderer_p){

	//renderer_p->scale = GUI_scale;
	Vec2f lastOffset = renderer_p->offset;
	renderer_p->offset = getVec2f(0.0);

	for(int i = 0; i < elements.length; i++){

		Element *element_p = &elements[i];
		
		if(element_p->type == ELEMENT_TYPE_RECT){

			Renderer2D_setShader(renderer_p, &renderer_p->colorShader);

			Renderer2D_setColor(renderer_p, element_p->color);

			Renderer2D_setAlpha(renderer_p, element_p->color.w);

			Renderer2D_setBorderRadius(renderer_p, element_p->borderRadius);

			//renderer_p->borderRadius = element_p->borderRadius;

			Renderer2D_drawRectangle(renderer_p, element_p->x, element_p->y, element_p->width, element_p->height);

		}
		
		if(element_p->type == ELEMENT_TYPE_TEXT){

			Renderer2D_setShader(renderer_p, &renderer_p->textureColorShader);

			Renderer2D_setColor(renderer_p, element_p->color);

			Renderer2D_setAlpha(renderer_p, element_p->color.w);

			Renderer2D_drawText(renderer_p, element_p->text, element_p->x, element_p->y, element_p->font_p, element_p->height);

		}

		if(element_p->type == ELEMENT_TYPE_TEXTURE){

			Renderer2D_setShader(renderer_p, &renderer_p->textureShader);

			Renderer2D_setTexture(renderer_p, &element_p->texture);

			Renderer2D_setColor(renderer_p, element_p->color);

			Renderer2D_setAlpha(renderer_p, element_p->color.w);

			Renderer2D_setBorderRadius(renderer_p, element_p->borderRadius);

			Renderer2D_drawRectangle(renderer_p, element_p->x, element_p->y, element_p->width, element_p->height);

		}

		if(element_p->type == ELEMENT_TYPE_ICON){

			Renderer2D_setShader(renderer_p, &renderer_p->iconShader);

			Renderer2D_setTexture(renderer_p, &element_p->texture);

			Renderer2D_setColor(renderer_p, GUI_color);
			Renderer2D_setColor2(renderer_p, GUI_backgroundColor);

			Renderer2D_setAlpha(renderer_p, element_p->color.w);

			Renderer2D_setBorderRadius(renderer_p, element_p->borderRadius);

			Renderer2D_drawRectangle(renderer_p, element_p->x, element_p->y, element_p->width, element_p->height);

		}

#ifndef __APPLE__
		if(element_p->type == ELEMENT_TYPE_COLOR_WHEEL){

			Renderer2D_setShader(renderer_p, &colorWheelShader);

			GL3D_uniformFloat(colorWheelShader.ID, "inputAngle", element_p->color.x);

			Renderer2D_drawRectangle(renderer_p, element_p->x, element_p->y, element_p->width, element_p->height);

		}
		
		if(element_p->type == ELEMENT_TYPE_COLOR_TRIANGLE){

			Renderer2D_setShader(renderer_p, &colorTriangleShader);

			GL3D_uniformFloat(colorTriangleShader.ID, "inputAngle", element_p->color.x);

			GL3D_uniformVec2f(colorTriangleShader.ID, "inputPos", getVec2f(element_p->color.y, element_p->color.z));

			Renderer2D_drawRectangle(renderer_p, element_p->x, element_p->y, element_p->width, element_p->height);
		
		}
#endif

	}

	renderer_p->offset = lastOffset;

}

void GUI_addTextElement(int x, int y, const char *text, int fontSize, GFont *font_p, Vec4f color){

	{
		Element element;

		element.type = ELEMENT_TYPE_TEXT;
		element.x = x;
		element.y = y;
		element.height = fontSize;
		element.color = color;
		element.font_p = font_p;
		element.text = string(text);

		elements.push(element);
	}
	
}

void GUI_addTextElementCentered(int x, int y, const char *text, int fontSize, GFont *font_p, Vec4f color){

	int width = GFont_getTextWidth(font_p, text, fontSize);

	GUI_addTextElement(x - width / 2, y, text, fontSize, font_p, color);

}

void GUI_addRectElement(int x, int y, int width, int height, Vec4f color){

	Element element;

	element.type = ELEMENT_TYPE_RECT;
	element.x = x;
	element.y = y;
	element.width = width;
	element.height = height;
	element.color = color;
	element.borderRadius = GUI_borderRadius;

	elements.push(element);

}

void GUI_addTextureElement(int x, int y, int width, int height, Texture texture, float alpha){

	Element element;

	element.type = ELEMENT_TYPE_TEXTURE;
	element.x = x;
	element.y = y;
	element.width = width;
	element.height = height;
	element.texture = texture;
	element.color = getVec4f(1.0, 1.0, 1.0, alpha);
	element.borderRadius = GUI_borderRadius;

	elements.push(element);
	
}

void GUI_addIconElement(int x, int y, int width, int height, Texture texture, float alpha){

	Element element;

	element.type = ELEMENT_TYPE_ICON;
	element.x = x;
	element.y = y;
	element.width = width;
	element.height = height;
	element.texture = texture;
	element.color = getVec4f(1.0, 1.0, 1.0, alpha);
	element.borderRadius = GUI_borderRadius;

	elements.push(element);
	
}

bool GUI_textButton(int x, int y, const char *text, int fontSize, GFont *font_p, bool active, const char *tag){

	bool returnValue = false;

	//Vec2f pointerPos = Engine_pointer.pos * ((float)WIDTH / (float)Engine_clientWidth);
	Vec2f pointerPos = getVec2f(Engine_pointer.pos.x * GUI_scale.y, Engine_pointer.pos.y * GUI_scale.y);

	int borderSize = fontSize / 20;
	int margin = fontSize / 20;
	float alpha = 1.0;

	if(active){
		alpha -= 0.3;
	}

	int width = GFont_getTextWidth(font_p, text, fontSize);
	int height = fontSize;

	bool hovered = false;
	if(pointerPos.x > x && pointerPos.x < x + width + borderSize * 2 + margin * 2
	&& pointerPos.y > y && pointerPos.y < y + height + borderSize * 2 + margin * 2){
		//alpha -= 0.5;
		hovered = true;
		GUI_isHovered = true;
		if(Engine_pointer.down){
			alpha -= 0.5;
		}
	}

	if(Engine_pointer.upped
	&& hovered
	&& strcmp(GUI_activeTag, tag) == 0){
		returnValue = true;
		String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
	}

	if(Engine_pointer.downed){
		if(strcmp(GUI_activeTag, tag) == 0){
			String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
		}
		if(hovered){
			String_set(GUI_activeTag, tag, SMALL_STRING_SIZE);
			GUI_textInputIsActive = false;
		}
	}

	//GUI_addRectElement(x, y, width + margin * 2 + borderSize * 2, (float)(height + margin * 2 + borderSize * 2) * 1.1, getVec4f(GUI_color.x, GUI_color.y, GUI_color.z, alpha));

	GUI_addRectElement(x, y, width + margin * 2 + borderSize * 2, height + margin * 2 + borderSize * 2, getVec4f(GUI_backgroundColor.x, GUI_backgroundColor.y, GUI_backgroundColor.z, alpha));
	//GUI_addRectElement(x, y, width + margin * 2 + borderSize * 2, height + margin * 2 + borderSize * 2, getVec4f(GUI_color.x, GUI_color.y, GUI_color.z, alpha));

	//GUI_addRectElement(x + borderSize, y + borderSize, width + margin * 2, height + margin * 2, getVec4f(GUI_backgroundColor.x, GUI_backgroundColor.y, GUI_backgroundColor.z, alpha));

	GUI_addTextElement(x + borderSize + margin, y + borderSize + margin, text, fontSize, font_p, getVec4f(GUI_color.x, GUI_color.y, GUI_color.z, alpha));

	return returnValue;

}

bool GUI_textButtonCentered(int x, int y, const char *text, int fontSize, GFont *font_p, bool active, const char *tag){

	int borderSize = fontSize / 10;
	int margin = fontSize / 10;

	return GUI_textButton(x - GFont_getTextWidth(font_p, text, fontSize) / 2.0 - borderSize - margin, y, text, fontSize, font_p, active, tag);

}

bool GUI_textureButton(int x, int y, int width, int height, Texture texture, bool active, const char *tag){

	bool returnValue = false;

	//Vec2f pointerPos = Engine_pointer.pos * ((float)WIDTH / (float)Engine_clientWidth);
	//Vec2f pointerPos = Engine_pointer.pos;
	Vec2f pointerPos = getVec2f(Engine_pointer.pos.x / GUI_scale.x, Engine_pointer.pos.y / GUI_scale.y);

	float alpha = 1.0;

	if(active){
		alpha -= 0.3;
	}

	bool hovered = false;
	if(pointerPos.x > x && pointerPos.x < x + width
	&& pointerPos.y > y && pointerPos.y < y + height){
		alpha -= 0.5;
		hovered = true;
		GUI_isHovered = true;
	}

	if(Engine_pointer.upped
	&& hovered
	&& strcmp(GUI_activeTag, tag) == 0){
		returnValue = true;
		String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
	}

	if(Engine_pointer.downed){
		if(strcmp(GUI_activeTag, tag) == 0){
			String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
		}
		if(hovered){
			String_set(GUI_activeTag, tag, SMALL_STRING_SIZE);
			GUI_textInputIsActive = false;
		}
	}

	GUI_addTextureElement(x, y, width, height, texture, alpha);

	return returnValue;

}

bool GUI_iconButton(int x, int y, int width, int height, Texture texture, bool active, const char *tag){

	bool returnValue = false;

	//Vec2f pointerPos = Engine_pointer.pos * ((float)WIDTH / (float)Engine_clientWidth);
	//Vec2f pointerPos = Engine_pointer.pos;
	Vec2f pointerPos = getVec2f(Engine_pointer.pos.x / GUI_scale.x, Engine_pointer.pos.y / GUI_scale.y);

	float alpha = 1.0;

	if(active){
		alpha -= 0.3;
	}

	bool hovered = false;
	if(pointerPos.x > x && pointerPos.x < x + width
	&& pointerPos.y > y && pointerPos.y < y + height){
		//alpha -= 0.5;
		hovered = true;
		GUI_isHovered = true;
		if(Engine_pointer.down){
			alpha -= 0.5;
		}
	}

	if(Engine_pointer.upped
	&& hovered
	&& strcmp(GUI_activeTag, tag) == 0){
		returnValue = true;
		String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
	}

	if(Engine_pointer.downed){
		if(strcmp(GUI_activeTag, tag) == 0){
			String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
		}
		if(hovered){
			String_set(GUI_activeTag, tag, SMALL_STRING_SIZE);
			GUI_textInputIsActive = false;
		}
	}

	//if(hovered
	//&& Engine_pointer.down){
		//alpha -= 0.5;
	//}

	//GUI_addRectElement(x, y, width, (float)height * 1.1, getVec4f(GUI_color.x, GUI_color.y, GUI_color.z, alpha));

	GUI_addIconElement(x, y, width, height, texture, alpha);

	return returnValue;

}

void GUI_textInput(int x, int y, int width, int fontSize, GFont *font_p, TextInputData *textInputData_p, const char *tag){

	//Vec2f pointerPos = Engine_pointer.pos * ((float)WIDTH / (float)Engine_clientWidth);
	Vec2f pointerPos = Engine_pointer.pos;

	float alpha = 1.0;

	if(strcmp(GUI_activeTag, tag) == 0){

		alpha -= 0.3;
		
		for(int i = 0; i < Engine_textInput.length; i++){

			if(Engine_textInput[i] == 8){//backspace
				if(strlen(textInputData_p->text) > 0){
					textInputData_p->text[strlen(textInputData_p->text) - 1] = *"\0";
				}
			}else{
				String_append_char(textInputData_p->text, Engine_textInput[i]);
			}

		}

	}

	bool hovered = false;

	if(pointerPos.x > x && pointerPos.x < x + width
	&& pointerPos.y > y && pointerPos.y < y + fontSize){
		hovered = true;
		GUI_isHovered = true;
	}

	if(hovered){
		alpha -= 0.5;

		if(Engine_pointer.downed){
			String_set(GUI_activeTag, tag, SMALL_STRING_SIZE);
			GUI_textInputIsActive = true;
		}

	}else{
		if(Engine_pointer.downed
		&& strcmp(GUI_activeTag, tag) == 0){
			String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
			GUI_textInputIsActive = false;
		}
	}

	GUI_addRectElement(x, y, width, fontSize, getVec4f(GUI_backgroundColor.x, GUI_backgroundColor.y, GUI_backgroundColor.z, alpha));
	
	GUI_addTextElement(x, y, textInputData_p->text, fontSize, font_p, getVec4f(GUI_color.x, GUI_color.y, GUI_color.z, alpha));

}

void GUI_colorPicker(int x, int y, int size, ColorPickerData *colorPickerData_p, const char *wheelTag, const char *triangleTag){

	//Vec2f pointerPos = Engine_pointer.pos * ((float)WIDTH / (float)Engine_clientWidth);
	//Vec2f pointerPos = Engine_pointer.pos;
	Vec2f pointerPos = getVec2f(Engine_pointer.pos.x / GUI_scale.x, Engine_pointer.pos.y / GUI_scale.y);

	Vec2f center = getVec2f(x + size / 2, y + size / 2);

	Vec2f relativePos = (pointerPos - center) / size;

	//handle wheel
	bool hovered = false;

	if(length(relativePos) < 0.4
	&& length(relativePos) > 0.3){
		hovered = true;
		GUI_isHovered = true;
	}

	float angle = -atan2(relativePos.y, relativePos.x);
	if(angle < 0.0){
		angle = M_PI * 2.0 + angle;
	}

	if(Engine_pointer.downed
	&& hovered){
		String_set(GUI_activeTag, wheelTag, SMALL_STRING_SIZE);
	}

	if(Engine_pointer.upped
	&& strcmp(GUI_activeTag, wheelTag) == 0){
		String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
		GUI_textInputIsActive = false;
	}

	if(strcmp(GUI_activeTag, wheelTag) == 0){
		colorPickerData_p->angle = angle;
	}

	{
		Element element;
		element.type = ELEMENT_TYPE_COLOR_WHEEL;
		element.x = x;
		element.y = y;
		element.width = size;
		element.height = size;

		element.color.x = colorPickerData_p->angle;

		elements.push(element);
	}

	//handle triangle
	x = x + size * 0.30;
	y = y + size * 0.25;
	size *= 0.4;
	center = getVec2f(x + size * 0.5, y + size * 0.5);
	relativePos = (pointerPos - center) / size;
	relativePos.y += 0.5;
	relativePos.x *= 2.0;

	hovered = false;

	if(relativePos.y >= 0.0 && relativePos.y <= 1.0
	&& fabs(relativePos.x) + 1.0 - relativePos.y <= 1.0
	|| length(relativePos - colorPickerData_p->pos) < 0.09){
		hovered = true;
		GUI_isHovered = true;
	}

	if(hovered){
		if(Engine_pointer.downed){
			String_set(GUI_activeTag, triangleTag, SMALL_STRING_SIZE);
			GUI_textInputIsActive = false;
		}
	}

	if(strcmp(GUI_activeTag, triangleTag) == 0){

		colorPickerData_p->pos = relativePos;

		if(colorPickerData_p->pos.y > 1.0){
			colorPickerData_p->pos.y = 1.0;
		}
		if(colorPickerData_p->pos.y < 0.0){
			colorPickerData_p->pos.y = 0.0;
		}
		if(colorPickerData_p->pos.x > colorPickerData_p->pos.y){
			colorPickerData_p->pos.x = colorPickerData_p->pos.y;
		}
		if(colorPickerData_p->pos.x < -colorPickerData_p->pos.y){
			colorPickerData_p->pos.x = -colorPickerData_p->pos.y;
		}

	}

	if(Engine_pointer.upped
	&& strcmp(GUI_activeTag, triangleTag) == 0){
		String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
	}

	{
		Element element;
		element.type = ELEMENT_TYPE_COLOR_TRIANGLE;
		element.x = x;
		element.y = y;
		element.width = size;
		element.height = size;

		element.color.x = colorPickerData_p->angle;
		element.color.y = colorPickerData_p->pos.x;
		element.color.z = colorPickerData_p->pos.y;

		elements.push(element);
	}

}

float SIXTH = M_PI * 2.0 / 6.0;

Vec4f colorFromColorPickerData(ColorPickerData colorPickerData){

	float angle = colorPickerData.angle;
	Vec2f pos = colorPickerData.pos;

	Vec3f rgb = getVec3f(0.0, 0.0, 0.0);

	rgb.x = (float)(angle < SIXTH) * angle / SIXTH +
			(float)(angle >= SIXTH && angle < SIXTH * 3) +
			(float)(angle >= SIXTH * 3 && angle < SIXTH * 4) * (SIXTH * 4 - angle) / SIXTH;

	rgb.y = (float)(angle >= SIXTH * 2 && angle < SIXTH * 3) * (angle - SIXTH * 2) / SIXTH +
			(float)(angle >= SIXTH * 3 && angle < SIXTH * 5) +
			(float)(angle >= SIXTH * 5 && angle < SIXTH * 6) * (SIXTH * 6 - angle) / SIXTH;

	rgb.z = (float)(angle >= SIXTH * 4 && angle < SIXTH * 5) * (angle - SIXTH * 4) / SIXTH +
			(float)(angle >= SIXTH * 5 && angle < SIXTH * 6 || angle >= 0 && angle < SIXTH) +
			(float)(angle >= SIXTH && angle < SIXTH * 2) * (SIXTH * 2 - angle) / SIXTH;

	float blackFactor = (pos.y - pos.x) / 2.0;
	float whiteFactor = (pos.x + pos.y) / 2.0;

	rgb = getVec3f(0.0, 0.0, 0.0) * blackFactor + getVec3f(1.0, 1.0, 1.0) * whiteFactor + rgb * (1.0 - blackFactor - whiteFactor);


	return getVec4f(rgb.x, rgb.y, rgb.z, 1.0);

}

ColorPickerData ColorPickerDataFromColor(Vec4f color){
	ColorPickerData tmp;
	return tmp;
}

void GUI_slider(int x, int y, int width, int height, float *factor_p, const char *tag){

	bool hovered = false;
	float alpha = 1.0;

	//Vec2f pointerPos = Engine_pointer.pos * ((float)WIDTH / (float)Engine_clientWidth);
	//Vec2f pointerPos = Engine_pointer.pos;
	Vec2f pointerPos = getVec2f(Engine_pointer.pos.x / GUI_scale.x, Engine_pointer.pos.y / GUI_scale.y);

	if(pointerPos.x > x && pointerPos.x < x + width
	&& pointerPos.y > y && pointerPos.y < y + height){
		hovered = true;
		GUI_isHovered = true;
	}

	if(hovered){

		alpha -= 0.5;

		if(Engine_pointer.downed){
			String_set(GUI_activeTag, tag, SMALL_STRING_SIZE);
			GUI_textInputIsActive = false;
		}

	}

	if(Engine_pointer.upped
	&& strcmp(GUI_activeTag, tag) == 0){
		String_set(GUI_activeTag, "", SMALL_STRING_SIZE);
	}

	if(strcmp(GUI_activeTag, tag) == 0){
		*factor_p = fmin(1.0, fmax(0.0, (float)(pointerPos.x - x) / (float)width));
		GUI_isHovered = true;
	}

	GUI_addRectElement(x, y + (float)height * 0.4, width, (float)height * 0.2, GUI_color);

	int knobWidth = width * 0.1;
	int knobX = x + *factor_p * (float)(width - knobWidth);

	GUI_addRectElement(knobX, y, knobWidth, height, GUI_backgroundColor);

}

bool GUI_keyboard_textButton(int x, int y, const char *text, int fontSize, GFont *font_p, const char *tag, const char *tagUp, const char *tagDown, const char *tagLeft, const char *tagRight){

	if(fontSize < 2){
		return false;
	}

	if(GUI_keyboard_nextShouldBecomeActive){
		GUI_keyboard_nextActiveTag = smallString(tag);
		GUI_keyboard_nextShouldBecomeActive = false;
	}

	GUI_keyboard_tagList.push(smallString(tag));

	bool returnValue = false;

	bool active = false;
	
	if(GUI_keyboard_activeTag == tag){
		active = true;
		if(actionKey.downed){
			returnValue = true;
		}
	}

	if(active){
		if(upKey.downed && tagUp != NULL){
			GUI_keyboard_nextActiveTag = smallString(tagUp);
			if(string(tagUp) == "LAST"){
				if(GUI_keyboard_tagList.length == 1){
					GUI_keyboard_lastShouldBecomeActive = true;
				}else{
					GUI_keyboard_nextActiveTag = GUI_keyboard_tagList[GUI_keyboard_tagList.length - 2];
				}
			}
		}
		if(downKey.downed && tagDown != NULL){
			GUI_keyboard_nextActiveTag = smallString(tagDown);
			if(string(tagDown) == "NEXT"){
				GUI_keyboard_nextShouldBecomeActive = true;
			}
		}
		if(leftKey.downed && tagLeft != NULL){
			GUI_keyboard_nextActiveTag = smallString(tagLeft);
		}
		if(rightKey.downed && tagRight != NULL){
			GUI_keyboard_nextActiveTag = smallString(tagRight);
		}
	}

	int borderSize = fontSize / 10;
	int margin = fontSize / 10;
	float alpha = 1.0;

	int width = GFont_getTextWidth(font_p, text, fontSize);
	int height = fontSize;

	Vec4f textColor = getVec4f(GUI_color.x, GUI_color.y, GUI_color.z, alpha);

	if(active){
		//textColor = GUI_backgroundColor * 0.5 + getVec4f(1.0) * 0.5;
		//alpha -= 0.3;
		GUI_addRectElement(x - borderSize, y - borderSize, width + margin * 2 + borderSize * 4, height + margin * 2 + borderSize * 4, GUI_backgroundColor * 0.5 + getVec4f(1.0) * 0.5);
	}

	GUI_addRectElement(x, y, width + margin * 2 + borderSize * 2, height + margin * 2 + borderSize * 2, getVec4f(GUI_backgroundColor.x, GUI_backgroundColor.y, GUI_backgroundColor.z, alpha));

	GUI_addTextElement(x + borderSize + margin, y + borderSize + margin, text, fontSize, font_p, textColor);

	return returnValue;
		
}

bool GUI_keyboard_textButton(int x, int y, const char *text, int fontSize, GFont *font_p, const char *tag){
	return GUI_keyboard_textButton(x, y, text, fontSize, font_p, tag, "LAST", "NEXT", NULL, NULL);
}

bool GUI_keyboard_textButtonCentered(int x, int y, const char *text, int fontSize, GFont *font_p, const char *tag, const char *tagUp, const char *tagDown, const char *tagLeft, const char *tagRight){

	bool returnValue = false;

	int borderSize = fontSize / 10;
	int margin = fontSize / 10;
	float alpha = 1.0;

	int width = GFont_getTextWidth(font_p, text, fontSize);

	return GUI_keyboard_textButton(x - width / 2 - margin - borderSize, y, text, fontSize, font_p, tag, tagUp, tagDown, tagLeft, tagRight);

}

bool GUI_keyboard_textButtonCentered(int x, int y, const char *text, int fontSize, GFont *font_p, const char *tag){
	return GUI_keyboard_textButtonCentered(x, y, text, fontSize, font_p, tag, "LAST", "NEXT", NULL, NULL);
}

void GUI_keyboard_slider(int x, int y, int width, int height, float *factor_p, const char *tag, const char *tagUp, const char *tagDown){

	if(width < 2 || height < 2){
		return;
	}

	if(GUI_keyboard_nextShouldBecomeActive){
		GUI_keyboard_nextActiveTag = smallString(tag);
		GUI_keyboard_nextShouldBecomeActive = false;
	}

	GUI_keyboard_tagList.push(smallString(tag));

	bool active = false;
	
	if(GUI_keyboard_activeTag == tag){
		active = true;
	}

	if(active){
		if(upKey.downed && tagUp != NULL){
			GUI_keyboard_nextActiveTag = smallString(tagUp);
			if(string(tagUp) == "LAST"){
				if(GUI_keyboard_tagList.length == 1){
					GUI_keyboard_lastShouldBecomeActive = true;
				}else{
					GUI_keyboard_nextActiveTag = GUI_keyboard_tagList[GUI_keyboard_tagList.length - 2];
				}
			}
		}
		if(downKey.downed && tagDown != NULL){
			GUI_keyboard_nextActiveTag = smallString(tagDown);
			if(string(tagDown) == "NEXT"){
				GUI_keyboard_nextShouldBecomeActive = true;
			}
		}
	}

	float slideSpeed = 0.015;

	if(active){
		if(leftKey.down){
			*factor_p += -slideSpeed;
		}
		if(rightKey.down){
			*factor_p += slideSpeed;
		}
		if(*factor_p < 0.0){
			*factor_p = 0.0;
		}
		if(*factor_p > 1.0){
			*factor_p = 1.0;
		}
	}

	float alpha = 1.0;
	float margin = height / 10;

	Vec4f highlightColor = GUI_backgroundColor * 0.5 + getVec4f(1.0) * 0.5;

	if(active){
		GUI_addRectElement(x - margin, y + (float)height * 0.4 - margin, width + margin * 2, (float)height * 0.2 + margin * 2, highlightColor);
	}

	GUI_addRectElement(x, y + (float)height * 0.4, width, (float)height * 0.2, GUI_color);

	int knobWidth = width * 0.1;
	int knobX = x + *factor_p * (float)(width - knobWidth);

	if(active){
		GUI_addRectElement(knobX - margin, y - margin, knobWidth + margin * 2, height + margin * 2, highlightColor);
	}

	GUI_addRectElement(knobX, y, knobWidth, height, GUI_backgroundColor);

}

void GUI_keyboard_slider(int x, int y, int width, int height, float *factor_p, const char *tag){
	GUI_keyboard_slider(x, y, width, height, factor_p, tag, "LAST", "NEXT");
}
