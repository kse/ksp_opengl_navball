SRC = main.cpp Camera.cpp Shader.cpp Telemachus.cpp Sphere.cpp ModelLoader.cpp\
	  lodepng.cpp Indicator.cpp
OBJ = $(SRC:.cpp=.o)
EXE = navball
CC = clang++

CFLAGS = -pipe -fno-exceptions -fstack-protector\
        -W -Wall -Wno-unused-parameter -std=c++11\
        -Wno-unused-function -Wno-unused-label -Wpointer-arith -Wformat\
        -Wreturn-type -Wsign-compare -Wmultichar -Wformat-nonliteral\
        -Winit-self -Wuninitialized -Wno-deprecated -Wformat-security -Werror\
        -pedantic -pedantic-errors ${EXEC} -fPIC\
        -fvisibility=hidden -O3

LFLAGS = -Wl,-z,relro -Wl,-z,now
LIBS = -lglfw -lGL -lGLEW -ljsoncpp -lassimp -lwebsockets

all: $(OBJ) $(EXE)

#This is the target that compiles our executable
$(EXE): $(OBJ)
	$(CC) $^ $(CFLAGS) $(LFLAGS) $(LIBS) -o $(EXE)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o
