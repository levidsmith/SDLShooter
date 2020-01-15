CC=gcc
EXE=sdl_shooter

$(EXE): sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c screen_game.c screen_title.c explosion.c screen_world_select.c stats.c enemy_golf.c enemy_hotel.c globals.h
	$(CC) -o $(EXE) sdl_shooter.c enemy.c level_reader.c ship.c bullet.c linked_list.c powerup.c screen_game.c screen_title.c explosion.c screen_world_select.c stats.c enemy_golf.c enemy_hotel.c `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_mixer -lm -mconsole

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
