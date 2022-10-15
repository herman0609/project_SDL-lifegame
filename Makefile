all:
	gcc -o gol screen.c main.c -IC:\MinGW\include -LC:\MinGW\lib -w -Wl, -lSDL2main -lSDL2