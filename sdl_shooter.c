//2019 Levi D. Smith - levidsmith.com
//New stuff
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "globals.h"
#include "linked_list.h"
#include "screen_game.h"


int iKeepLooping = TRUE;

//SDL variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


SDL_Surface* screenSurface = NULL;

SDL_Texture *imgShip;
SDL_Texture *imgEnemyAlpha_00;
SDL_Texture *imgEnemyAlpha_01;
SDL_Texture *imgEnemyBravo_00;
SDL_Texture *imgEnemyBravo_01;
SDL_Texture *imgEnemyCharlie_00;
SDL_Texture *imgEnemyCharlie_01;
SDL_Texture *imgEnemyDelta_00;
SDL_Texture *imgEnemyDelta_01;


SDL_Texture *imgBackground;
SDL_Texture *imgBullet;
SDL_Texture *imgBulletEnemy;
SDL_Texture *imgScoreText;
SDL_Texture *imgLevelCompleteText;
SDL_Texture *imgGameOverText;
SDL_Texture *imgLevel;
SDL_Texture *imgPowerupAlpha;
SDL_Texture *imgTitleText;
SDL_Texture *imgCopyrightText;


TTF_Font *fontDefault;
TTF_Font *fontLarge;

Mix_Chunk *soundShoot;
Mix_Chunk *soundShipDead;
Mix_Chunk *soundEnemyDead;
Mix_Chunk *soundEnemyShoot;
Mix_Chunk *soundPowerup;

Mix_Music *musicGame; 
Mix_Music *musicTitle; 

//VARIABLES

char strFPS[64];
Uint32 iTime;
Uint32 iDelay;
int iFrames;
int iFPS;

int iCurrentScreen = 0;


//FUNCTION PROTOTYPES
extern void start_screen_game();
extern void update_screen_game();
extern void draw_screen_game();
extern void handleInput_screen_game(int iType, int iKey);


extern void start_screen_title();
extern void update_screen_title();
extern void draw_screen_title();
extern void handleInput_screen_title(int iType, int iKey);


//FUNCTIONS

void start() {
	switch(iCurrentScreen) {
		case 0:
			start_screen_title();
			break;
		case 1:
			start_screen_game();
			break;
	}
  
}

void update() {
	switch(iCurrentScreen) {
		case 0:
			update_screen_title();
			break;
		case 1:
			update_screen_game();
			break;
	}
}



void draw() {
	switch(iCurrentScreen) {
		case 0:
			draw_screen_title();
			break;
		case 1:
			draw_screen_game();
			break;
	}
	

}

void handleInput(int iType, int iKey) {
	switch(iCurrentScreen) {
		case 0:
			handleInput_screen_title(iType, iKey);
			break;
		case 1:
			handleInput_screen_game(iType, iKey);
			break;
	}

	
}

void setCurrentScreen(int iScreen) {
	iCurrentScreen = iScreen;

	switch(iCurrentScreen) {
		case 0:
			start_screen_title();
			break;
		case 1:
			start_screen_game();
			break;
	}

}

void quit() {
	iKeepLooping = FALSE;
}



int main(int argc, char* args[]) {

  //seed randomizer
  srand(time(NULL));
//  printf("random number %d\n", rand());

//  if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
  if (SDL_Init( SDL_INIT_VIDEO) < 0) {
    printf("Error: %s\n", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow("SDL Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Error: %s\n", SDL_GetError());
    return 1;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//  screenSurface = SDL_GetWindowSurface(window);




  //load media  
  SDL_Surface* sprShip;
  SDL_Surface* sprEnemy;
  SDL_Surface* sprBackground;
  SDL_Surface* sprBullet;
  SDL_Surface  *sprPowerup;

  
  sprShip = SDL_LoadBMP("assets/images/ship.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  imgShip = SDL_CreateTextureFromSurface(renderer, sprShip);
  
  sprBackground = SDL_LoadBMP("assets/images/background.bmp");
  imgBackground = SDL_CreateTextureFromSurface(renderer, sprBackground);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_alpha1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyAlpha_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_alpha2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyAlpha_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_bravo1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyBravo_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_bravo2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyBravo_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_charlie1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyCharlie_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_charlie2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyCharlie_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_delta1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyDelta_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_delta2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyDelta_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);


  sprBullet = SDL_LoadBMP("assets/images/bullet.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBullet = SDL_CreateTextureFromSurface(renderer, sprBullet);

  sprBullet = SDL_LoadBMP("assets/images/bullet_enemy.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBulletEnemy = SDL_CreateTextureFromSurface(renderer, sprBullet);

  sprPowerup = SDL_LoadBMP("assets/images/powerup_alpha.bmp");
  SDL_SetColorKey(sprPowerup, SDL_TRUE, SDL_MapRGB(sprPowerup->format, 255, 0, 255));
  imgPowerupAlpha = SDL_CreateTextureFromSurface(renderer, sprPowerup);
  
  printf("created textures\n");


  SDL_FreeSurface(sprShip);
  SDL_FreeSurface(sprBackground);
  SDL_FreeSurface(sprBullet);
  SDL_FreeSurface(sprPowerup);


//handle loading fonts
  if (TTF_Init() == -1) {
    exit(1);
  }
  fontDefault = TTF_OpenFont("SudburyBasin-Regular.ttf", 20);
  fontLarge = TTF_OpenFont("SudburyBasin-Regular.ttf", 64);
  
  if (!fontDefault || !fontLarge) {
	  printf("Errors loading font\n");
  }

 
//handle loading sounds
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
  soundShoot = Mix_LoadWAV("assets/audio/shoot.wav");
  soundEnemyDead = Mix_LoadWAV("assets/audio/enemy_dead.wav");
  soundEnemyShoot = Mix_LoadWAV("assets/audio/enemy_shoot.wav");
  soundShipDead = Mix_LoadWAV("assets/audio/ship_dead.wav");
  soundPowerup = Mix_LoadWAV("assets/audio/powerup.wav");

  musicGame = Mix_LoadMUS("assets/audio/sdl-shooter-level.wav");
  musicTitle = Mix_LoadMUS("assets/audio/sdl-shooter-title.wav");



  start();

  iTime = SDL_GetTicks();
  
 // while (iKeepLooping == TRUE) {
	while(1) {
      SDL_Event theEvent;

    update();

    draw();
	SDL_RenderPresent(renderer);

	if (SDL_PollEvent(&theEvent)) {
//			printf("SDL_PollEvent\n");
			

      if (theEvent.type  == SDL_QUIT) {
		printf("Close button pressed\n");
		printf("Stop looping\n");
		break;
		printf("Shouldn't get here\n");
      } else if (theEvent.type == SDL_KEYDOWN || theEvent.type == SDL_KEYUP) {
        handleInput(theEvent.type, theEvent.key.keysym.sym);  
      }

    }
	
	if (!iKeepLooping) {
		break;
	}
	

    //Set target frame rate
	/*
    iDelay = (1000 * DELTA_TIME) - (SDL_GetTicks() - iTime);
	if (iDelay < 0) {
		iDelay = 0;
	}
    SDL_Delay(iDelay);
	iFPS = 1000 / (SDL_GetTicks() - iTime);
	iTime = SDL_GetTicks();
	*/
	SDL_Delay(16);  //just set this to a constant for now.  seems to resolve freezes
  }
  
  printf("Stopped looping\n");

  TTF_Quit();


  Mix_FreeChunk(soundShoot);
  Mix_FreeChunk(soundEnemyDead);
  Mix_FreeChunk(soundEnemyShoot);
  Mix_FreeChunk(soundShipDead);
  Mix_FreeChunk(soundPowerup);

  Mix_FreeMusic(musicGame);
  Mix_FreeMusic(musicTitle);
  

  Mix_CloseAudio();

  
  SDL_DestroyTexture(imgShip);
  SDL_DestroyTexture(imgBackground);
  SDL_DestroyTexture(imgEnemyAlpha_00);
  SDL_DestroyTexture(imgEnemyAlpha_01);
  SDL_DestroyTexture(imgEnemyBravo_00);
  SDL_DestroyTexture(imgEnemyBravo_01);
  SDL_DestroyTexture(imgEnemyCharlie_00);
  SDL_DestroyTexture(imgEnemyCharlie_01);
  SDL_DestroyTexture(imgEnemyDelta_00);
  SDL_DestroyTexture(imgEnemyDelta_01);
  
  SDL_DestroyTexture(imgBullet);
  SDL_DestroyTexture(imgScoreText);
  SDL_DestroyTexture(imgLevelCompleteText);
  SDL_DestroyTexture(imgLevel);

  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
