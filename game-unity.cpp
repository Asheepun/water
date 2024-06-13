#ifdef __linux__
#ifndef __ANDROID__
#include "lib/engine/engine-unity-linux.cpp"
#endif
#endif

#ifdef __ANDROID__
#include "lib/engine/engine-unity-android.cpp"
#endif

#ifdef __ANDROID__
#define POINTER_CONTROLS
#endif

#include "main.cpp"
