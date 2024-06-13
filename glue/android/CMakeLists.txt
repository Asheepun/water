# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("picto")

include_directories(experiment/include)

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(picto SHARED
        experiment/lib/engine/android/main.cpp
        experiment/lib/engine/android/AndroidOut.cpp
        experiment/lib/engine/android/audio.cpp
        experiment/lib/engine/engine.cpp
        experiment/lib/engine/geometry.cpp
        experiment/lib/engine/renderer2d.cpp
        experiment/lib/engine/strings.cpp
        experiment/lib/engine/text.cpp
        experiment/lib/engine/shaders.cpp
        experiment/lib/engine/gl3d.cpp
        experiment/lib/engine/3d-no-driver.cpp
        experiment/lib/engine/files.cpp
        experiment/lib/engine/gui.cpp
        experiment/lib/engine/shaders.cpp
        experiment/lib/engine/audio.cpp

        experiment/main.cpp
        )

# Searches for a package provided by the game activity dependency
find_package(game-activity REQUIRED CONFIG)

# Configure libraries CMake uses to link your target library.
target_link_libraries(picto
        # The game activity
        game-activity::game-activity
        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        EGL
        GLESv3
        jnigraphics
        android
        log
        aaudio
        camera2ndk
        mediandk)
