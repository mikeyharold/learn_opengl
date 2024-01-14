INCs=-I/home/baines/jects/learn_opengl/glfw-3.3.9/include \
-I/home/baines/jects/learn_opengl/glad/include \
-I/home/baines/jects/learn_opengl/glfw-3.3.9/deps

LIBs=-L/home/baines/jects/learn_opengl/glfw-3.3.9/build/src/
lIBs=-lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm

main : main.c glad.c
	gcc $^ -o $@ -O2 -march=native ${INCs} ${LIBs} ${lIBs}
