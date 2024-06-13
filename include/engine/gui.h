struct ColorPickerData{
	float angle;
	Vec2f pos;
};

struct TextInputData{
	String text;
	//char text[STRING_SIZE];
	int cursorIndex;
};

extern SmallString GUI_activeTag;
extern SmallString GUI_keyboard_activeTag;
extern SmallString GUI_keyboard_nextActiveTag;
//extern int GUI_activeID;
extern bool GUI_isHovered;
extern bool GUI_textInputIsActive;


extern float GUI_borderRadius;
extern Vec4f GUI_color;
extern Vec4f GUI_backgroundColor;
extern Vec2f GUI_scale;
