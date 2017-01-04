
all:
	gcc -std=c99 -Wall -pedantic -lm -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_gfx main.c -omain
