CC=gcc
EXE=sdl_shooter

$(EXE): sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c screen_game.c screen_title.c 
	$(CC) -o $(EXE) sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c screen_game.c screen_title.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -lm

clean:
	rm sdl_shooter.exe
	rm *.o

linux:
	$(CC) -o $(EXE) sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c screen_game.c screen_title.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -lm

window_only: window_only.c
	gcc -o window_only window_only.c `sdl2-config --cflags --libs` -mconsole
