clang lib/glad/glx.c lib/glad/gl.c game-unity.cpp -O3 -I ./include/ -ldl -lm -lGL -lX11 -lXfixes -lpthread -mavx -lpulse -lpulse-simple -lvulkan -Wno-logical-op-parentheses -o main && ./main
