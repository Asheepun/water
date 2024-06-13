#define STRING_SIZE 256
#define LARGE_STRING_SIZE 1024
#define SMALL_STRING_SIZE 32

struct String{
	char value[STRING_SIZE];

	operator char *();
	operator const char *();
	char& operator[] (int);
	bool operator== (const char *);
	bool operator!= (const char *);
	String operator/ (const char *);
	void operator/= (const char *);
};

struct SmallString{
	char value[SMALL_STRING_SIZE];

	operator char *();
	operator const char *();
	char& operator[] (int);
	bool operator== (const char *);
	bool operator!= (const char *);
};

struct UnicodeCharacter{
	char value[5];

	operator char *();
	operator const char *();
	char& operator[] (int);
	bool operator== (const char *);
	bool operator!= (const char *);
};
