# Makefile for menger sponge code

all: menger.cpp
	g++ -o menger menger.cpp -lGL -lGLU -lglut

clean:
	$(RM) menger