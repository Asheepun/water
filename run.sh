glslc -fshader-stage=compute shader.glsl -o shader.spv

clang lib/glad/glx.c lib/glad/gl.c game-unity.cpp -g -I ./include/ -ldl -lm -lGL -lX11 -lXfixes -lpthread -mavx -lpulse -lpulse-simple -lvulkan -Wno-logical-op-parentheses -o main && ./main
