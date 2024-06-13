#include "engine/strings.h"

//operator overloading for String
String::operator char *(){
   return value; 
}

String::operator const char *(){ 
   return value; 
}

char& String::operator[](int i){
	return value[i];
}

bool String::operator==(const char *otherString){
	return strcmp(value, otherString) == 0;
}

bool String::operator!=(const char *otherString){
	return strcmp(value, otherString) != 0;
}

String String::operator/(const char *appendString){
	String newString;
	memset(newString, 0, STRING_SIZE);
	memcpy(newString, value, strlen(value));
	memcpy(newString.value + strlen(newString), appendString, strlen(appendString));
	return newString;
}

void String::operator/=(const char *appendString){
	memcpy(value + strlen(value), appendString, strlen(appendString));
}

//operator overloading for SmallString
SmallString::operator char*(){ 
   return value; 
}

SmallString::operator const char*(){ 
   return value; 
}

char& SmallString::operator[](int i){
	return value[i];
}

bool SmallString::operator==(const char *otherString){
	return strcmp(value, otherString) == 0;
}
bool SmallString::operator!=(const char *otherString){
	return strcmp(value, otherString) != 0;
}

//operator overloading for UnicodeCharacter
UnicodeCharacter::operator char*(){ 
   return value; 
}

UnicodeCharacter::operator const char*(){ 
   return value; 
}

char& UnicodeCharacter::operator[](int i){
	return value[i];
}

bool UnicodeCharacter::operator==(const char *otherString){
	return strcmp(value, otherString) == 0;
}
bool UnicodeCharacter::operator!=(const char *otherString){
	return strcmp(value, otherString) != 0;
}

//regular functions
void String_set(char *destString, const char *sourceString, int size){

	memset(destString, 0, size);

	if(strlen(sourceString) >= size){
		printf("SOURCE STRING TO BIG!\n");
		printf("%s\n", sourceString);
	}

	memcpy(destString, sourceString, strlen(sourceString));
	
}

void String_append(char *baseString, const char *appendString){

	memcpy(baseString + strlen(baseString), appendString, strlen(appendString));

}

void String_append_char(char *baseString, char character){
	
	baseString[strlen(baseString) + 1] = *"\0";
	baseString[strlen(baseString)] = character;
	//memcpy(baseString + strlen(baseString), appendString, strlen(appendString));

}

void String_append_int(char *baseString, int value){

	char buff[STRING_SIZE];
	memset(buff, 0, STRING_SIZE);

	sprintf(buff, "%i", value);

	memcpy(baseString + strlen(baseString), buff, strlen(buff));

}

void String_append_float(char *baseString, float value){

	char buff[STRING_SIZE];
	memset(buff, 0, STRING_SIZE);

	sprintf(buff, "%f", value);

	memcpy(baseString + strlen(baseString), buff, strlen(buff));

}

void String_clearRange(char *string, int startIndex, int stopIndex){

	memset(string + startIndex, 0, stopIndex - startIndex);

}

String string(const char *inputString){
	String newString;
	String_set(newString, inputString, STRING_SIZE);
	return newString;
}

String string(float x){
	String newString;
	memset(newString, 0, STRING_SIZE);

	sprintf(newString, "%f", x);

	while(strlen(newString) > 3
	&& newString[strlen(newString) - 1] == *"0"){
		newString[strlen(newString) - 1] = 0;
	}

	return newString;
}

String string(int x){
	String newString;
	memset(newString, 0, STRING_SIZE);
	sprintf(newString, "%i", x);
	return newString;
}

SmallString smallString(const char *inputString){
	SmallString newString;
	String_set(newString, inputString, SMALL_STRING_SIZE);
	return newString;
}

Array<UnicodeCharacter> getUnicodeCharacters_mustFree(const char *string){

	Array<UnicodeCharacter> characters;
	characters.init();

	int index = 0;

	while(index < strlen(string)){

		UnicodeCharacter character;
		memset(character.value, 0, 5);
		int n_bytes = 1;

		n_bytes += (int)(string[index] < 0) * (1 + (int)(string[index] >= -32) + (int)(string[index] >= -16));

		memcpy(character.value, string + index, n_bytes);

		characters.push(character);

		index += n_bytes;

	}

	return characters;

}

int getCodePoint(UnicodeCharacter character){
	if(character[0] > 0){
		return (unsigned char)character[0];
	}else{
		if(character[0] >= -16){
			return ((unsigned char)character[3] % 64) + 64 * 64 * ((unsigned char)character[2] % 64) + 64 * 64 * ((unsigned char)character[1] % 64) + 64 * 64 * 64 * ((unsigned char)character[0] % 16);
		}
		if(character[0] >= -32){
			return ((unsigned char)character[2] % 64) + 64 * ((unsigned char)character[1] % 64) + 64 * 64 * ((unsigned char)character[0] % 32);
		}
		return ((unsigned char)character[1] % 64) + 64 * ((unsigned char)character[0] % 64);
	}
}

void String_log(SmallString string){
	printf("%s\n", (char *)string);
}

float toFloat(const char *string, char **next_p){
	return strtof(string, next_p);
}

float toFloat(const char *string){
	char *next_p;
	return strtof(string, &next_p);
}

int toInt(const char *string, char **next_p){
	return strtol(string, next_p, 10);
}

int toInt(const char *string){
	char *next_p;
	return strtol(string, &next_p, 10);
}

Vec3f toVec3f(const char *string){

	Vec3f v;
	char *ptr;

	v.x = toFloat(string, &ptr);
	v.y = toFloat(ptr, &ptr);
	v.z = toFloat(ptr, &ptr);

	return v;

}

void print(String text){
	printf("%s\n", text.value);
}

void print(SmallString text){
	printf("%s\n", text.value);
}

void print(UnicodeCharacter character){
	printf("%s\n", character.value);
}

void print(const char *text){
	printf("%s\n", text);
}

void print(int x){
	printf("%i\n", x);
}

void print(float x){
	printf("%f\n", x);
}
void print(bool x){
	if(x){
		printf("true\n");
	}else{
		printf("false\n");
	}
}
