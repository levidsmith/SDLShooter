CC=gcc
EXE=sdl_shooter
OBJS = sdl_shooter.o enemy.o level_reader.o ship.o bullet.o linked_list.o powerup.o screen_game.o screen_title.o explosion.o screen_world_select.o stats.o util.o enemy_golf.o enemy_hotel.o enemy_india.o enemy_juliett.o enemy_kilo.o enemy_lima.o enemy_mike.o
DEPS = globals.h util.h

%.o: %.c $(DEPS)
	$(CC) `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -lm -mconsole -c -o $@ $<

$(EXE): $(OBJS)
	$(CC) -o $@ $^ `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -lm -mconsole

all: sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c screen_game.c screen_title.c explosion.c screen_world_select.c stats.c util.c enemy_golf.c enemy_hotel.c enemy_india.c enemy_juliett.c enemy_kilo.c enemy_lima.c enemy_mike.c globals.h
	$(CC) -o $(EXE) sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c screen_game.c screen_title.c explosion.c screen_world_select.c stats.c util.c enemy_golf.c enemy_hotel.c enemy_india.c enemy_juliett.c enemy_kilo.c enemy_lima.c enemy_mike.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -lm -mconsole

release:
	rm -rf build/SDLShooterWindows
	mkdir -p build/SDLShooterWindows
	mkdir -p build/SDLShooterWindows/assets/images
	mkdir -p build/SDLShooterWindows/assets/audio
	cp sdl_shooter.exe build/SDLShooterWindows
	cp dll/*.dll build/SDLShooterWindows
	cp assets/images/*.bmp build/SDLShooterWindows/assets/images
	cp assets/audio/*.wav build/SDLShooterWindows/assets/audio
	cp assets/level*.txt build/SDLShooterWindows/assets
	cp assets/*.ttf build/SDLShooterWindows/assets

clean:
	rm sdl_shooter.exe
	rm *.o

linux:
	$(CC) -o $(EXE) sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c screen_game.c screen_title.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -lm

window_only: window_only.c
	gcc -o window_only window_only.c `sdl2-config --cflags --libs` -mconsole
