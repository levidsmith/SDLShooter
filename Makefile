CC=gcc
EXE=sdl_shooter

$(EXE): sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c 
	$(CC) -o $(EXE) sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -mconsole

linux:
	$(CC) -o $(EXE) sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer

window_only: window_only.c
	gcc -o window_only window_only.c `sdl2-config --cflags --libs` -mconsole
