//2019 Levi D. Smith

#ifndef globals_h
#define globals_h

#define TRUE 1
#define FALSE 0

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define FPS 60.0
#define DELTA_TIME (1.0 / FPS)


#define UNIT_SIZE 64
#define PI 3.14159265

#define LEVEL_FILE "assets/level_00.txt"

//MinGW style
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
//MacOS - XCode Style
//#include <SDL2/SDL.h>
//#include <SDL2_ttf/SDL_ttf.h>
//#include <SDL2_mixer/SDL_mixer.h>

#endif
