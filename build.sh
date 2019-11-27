#!/bin/bash
#gcc -o test_game test_game.c `sdl2-config --cflags --libs`
gcc -o test_game test_game.c `sdl2-config --cflags --libs` -lSDL2_ttf
