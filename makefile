SRC = main.cpp unit.cpp common.cpp manage.cpp squadrons.cpp
DEPS = $(addprefix src/,$(SRC))
INCL = main.hpp unit.hpp common.hpp manage.hpp squadrons.hpp
LIB = -lconfig++ -fopenmp -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lopengl32 -lgdi32 -lwinmm -lfreetype -lm
LIB_PATH = -I ./deps/include -L ./deps/lib
OBJ = $(SRC:.cpp=.o)
EXE = debug/warstick.exe
RELEASE_EXE = release/warstick.exe

CC = g++
CFLAGS = -std=c++11 -Wall
LDFLAGS = -o $(EXE)
CFDEBUG = -g -D DEBUG -D SFML_STATIC
RELEASE_CFLAGS = -O2 -s -mwindows -static-libgcc -static-libstdc++

$(EXE): $(DEPS)
	g++ -o $@ $^ $(CFLAGS) $(CFDEBUG) $(LIB_PATH) $(LIB)

distr:
	g++ -o $(RELEASE_EXE) $(DEPS) $(CFLAGS) $(RELEASE_CFLAGS) $(LIB_PATH) $(LIB)