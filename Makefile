CC=gcc
EXE=sdl_shooter

$(EXE): sdl_shooter.c enemy.c level_reader.c ship.c 
	$(CC) -o $(EXE) sdl_shooter.c enemy.c level_reader.c ship.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -mconsole
