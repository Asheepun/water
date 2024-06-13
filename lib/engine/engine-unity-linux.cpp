#include "math.h"
#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "dirent.h"
#include "errno.h"
#include "stdbool.h"
#include "stddef.h"
#include "time.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "stb/stb_truetype.h"

#include "glad/gl.h"

#include "engine/assetManager.h"

#include "array.cpp"
#include "time.cpp"
#include "geometry.cpp"
#include "strings.cpp"
#include "files.cpp"
#include "shaders.cpp"
#include "3d-no-driver.cpp"
#include "gl3d.cpp"
#include "gvk.cpp"
#include "text.cpp"
#include "engine.cpp"
#include "engine-linux.cpp"
#include "renderer2d.cpp"
#include "gui.cpp"
#include "audio.cpp"
#include "audio-linux.cpp"
