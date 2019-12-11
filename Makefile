CC=gcc
EXE=sdl_shooter

$(EXE): sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c 
	$(CC) -o $(EXE) sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -mconsole

clean:
	rm sdl_shooter.exe
	rm *.o

linux:
	$(CC) -o $(EXE) sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer

window_only: window_only.c
	gcc -o window_only window_only.c `sdl2-config --cflags --libs` -mconsole
