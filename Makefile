OBJS = main.cpp Camera.cpp Shader.cpp Telemachus.cpp Sphere.cpp ModelLoader.cpp
OBJ_NAME = navball
CC = clang++

CFLAGS = -pipe -fno-exceptions -fstack-protector -Wl,-z,relro -Wl,-z,now\
        -W -Wall -Wno-unused-parameter -std=c++11\
        -Wno-unused-function -Wno-unused-label -Wpointer-arith -Wformat\
        -Wreturn-type -Wsign-compare -Wmultichar -Wformat-nonliteral\
        -Winit-self -Wuninitialized -Wno-deprecated -Wformat-security -Werror\
        -pedantic -pedantic-errors ${EXEC} -fPIC\
        -fvisibility=hidden -O3

LFLAGS = -lglfw -lGL -lGLEW -lcurl -ljsoncpp -lassimp

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) -o $(OBJ_NAME)
