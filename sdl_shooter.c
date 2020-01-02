//2019 Levi D. Smith - levidsmith.com
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linked_list.h"
#include "screen_game.h"
#include "screen_world_select.h"


int iKeepLooping = TRUE;

//SDL variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


SDL_Surface* screenSurface = NULL;

SDL_Texture *imgShip[NUM_WEAPONS];
SDL_Texture *imgEnemyAlpha_L1_00;
SDL_Texture *imgEnemyAlpha_L1_01;
SDL_Texture *imgEnemyAlpha_L2_00;
SDL_Texture *imgEnemyAlpha_L2_01;
SDL_Texture *imgEnemyBravo_L1_00;
SDL_Texture *imgEnemyBravo_L1_01;
SDL_Texture *imgEnemyBravo_L2_00;
SDL_Texture *imgEnemyBravo_L2_01;
SDL_Texture *imgEnemyCharlie_L1_00;
SDL_Texture *imgEnemyCharlie_L1_01;
SDL_Texture *imgEnemyCharlie_L2_00;
SDL_Texture *imgEnemyCharlie_L2_01;
SDL_Texture *imgEnemyDelta_00;
SDL_Texture *imgEnemyDelta_01;
SDL_Texture *imgEnemyEcho_00;
SDL_Texture *imgEnemyEcho_01;
SDL_Texture *imgEnemyEcho_02;
SDL_Texture *imgEnemyEcho_03;
SDL_Texture *imgExplosion_L2_00;
SDL_Texture *imgExplosion_L2_01;
SDL_Texture *imgFireButton[16];
SDL_Texture *imgFireButtonText[3];
SDL_Texture *imgHealthUnit[5];



SDL_Texture *imgBackground[NUM_WORLDS * 2];
SDL_Texture *imgBullet[6];
SDL_Texture *imgBulletEnemy;
SDL_Texture *imgScoreText;
SDL_Texture *imgLevelCompleteText;
SDL_Texture *imgGameOverText;
SDL_Texture *imgLevel;
SDL_Texture *imgWeaponText;
SDL_Texture *imgPowerup[NUM_POWERUPS * 2];
SDL_Texture *imgShipPowerup[NUM_SHIP_POWERUPS];
SDL_Texture *imgTitleText;
SDL_Texture *imgCopyrightText;
SDL_Texture *imgGameTimeText;
SDL_Texture *imgTitleStartText;
SDL_Texture *imgTitleContinueText;
SDL_Texture *imgTitleQuitText;

SDL_Texture *imgWorldSelectText;
SDL_Texture *imgWorldSelectSelectedText;
SDL_Texture *imgWorldSelectWorldText[NUM_WORLDS];


TTF_Font *fontDefault;
TTF_Font *fontLarge;

Mix_Chunk *soundShoot;
Mix_Chunk *soundShipHit;
Mix_Chunk *soundShipDead;
Mix_Chunk *soundEnemyDead;
Mix_Chunk *soundEnemyShoot;
Mix_Chunk *soundEnemyHit;
Mix_Chunk *soundEnemyShield;
Mix_Chunk *soundPowerup;
Mix_Chunk *soundWeaponSelect;

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
		case 2:
			start_screen_world_select();
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
		case 2:
			update_screen_world_select();
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
		case 2:
			draw_screen_world_select();
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
		case 2:
			handleInput_screen_world_select(iType, iKey);
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
		case 2:
			start_screen_world_select();
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
  SDL_Surface *sprExplosion;
  SDL_Surface *sprFireButton;
  SDL_Surface *spr;


int i;
char strFile[32];


for (i = 0; i < NUM_WEAPONS; i++) {
	sprintf(strFile, "assets/images/ship%d.bmp", (i + 1));
  spr = SDL_LoadBMP(strFile);
  SDL_SetColorKey(spr, SDL_TRUE, SDL_MapRGB(spr->format, 255, 0, 255));
  imgShip[i] = SDL_CreateTextureFromSurface(renderer, spr);
  SDL_FreeSurface(spr);
	
}


  
/*
  sprShip = SDL_LoadBMP("assets/images/ship1.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  imgShip[0] = SDL_CreateTextureFromSurface(renderer, sprShip);
  SDL_FreeSurface(sprShip);

  sprShip = SDL_LoadBMP("assets/images/ship2.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  imgShip[1] = SDL_CreateTextureFromSurface(renderer, sprShip);
  SDL_FreeSurface(sprShip);
  
  sprShip = SDL_LoadBMP("assets/images/ship3.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  imgShip[2] = SDL_CreateTextureFromSurface(renderer, sprShip);
  SDL_FreeSurface(sprShip);
  
  sprShip = SDL_LoadBMP("assets/images/ship4.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  imgShip[3] = SDL_CreateTextureFromSurface(renderer, sprShip);
  SDL_FreeSurface(sprShip);
  
  sprShip = SDL_LoadBMP("assets/images/ship5.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  imgShip[4] = SDL_CreateTextureFromSurface(renderer, sprShip);
  SDL_FreeSurface(sprShip);
  
  sprShip = SDL_LoadBMP("assets/images/ship6.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  imgShip[5] = SDL_CreateTextureFromSurface(renderer, sprShip);
  SDL_FreeSurface(sprShip);
  */

for (i = 0; i < NUM_WORLDS * 2; i++) {
	sprintf(strFile, "assets/images/background%d.bmp", (i + 1));
  
  sprBackground = SDL_LoadBMP(strFile);
  imgBackground[i] = SDL_CreateTextureFromSurface(renderer, sprBackground);

}
  sprEnemy = SDL_LoadBMP("assets/images/enemy_alpha1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyAlpha_L1_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_alpha2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyAlpha_L1_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_alpha_l2_1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyAlpha_L2_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_alpha_l2_2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyAlpha_L2_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);


  sprEnemy = SDL_LoadBMP("assets/images/enemy_bravo_l1_1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyBravo_L1_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_bravo_l1_2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyBravo_L1_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_bravo_l2_1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyBravo_L2_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_bravo_l2_2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyBravo_L2_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_charlie_l1_1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyCharlie_L1_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_charlie_l1_2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyCharlie_L1_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_charlie_l2_1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyCharlie_L2_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_charlie_l2_2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyCharlie_L2_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);


  sprEnemy = SDL_LoadBMP("assets/images/enemy_delta1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyDelta_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_delta2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyDelta_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_echo1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyEcho_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_echo2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyEcho_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_echo3.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyEcho_02 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_echo4.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyEcho_03 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);


  sprBullet = SDL_LoadBMP("assets/images/bullet1.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBullet[0] = SDL_CreateTextureFromSurface(renderer, sprBullet);
  SDL_FreeSurface(sprBullet);

  sprBullet = SDL_LoadBMP("assets/images/bullet2.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBullet[1] = SDL_CreateTextureFromSurface(renderer, sprBullet);
  SDL_FreeSurface(sprBullet);

  sprBullet = SDL_LoadBMP("assets/images/bullet3.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBullet[2] = SDL_CreateTextureFromSurface(renderer, sprBullet);
  SDL_FreeSurface(sprBullet);

  sprBullet = SDL_LoadBMP("assets/images/bullet4.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBullet[3] = SDL_CreateTextureFromSurface(renderer, sprBullet);
  SDL_FreeSurface(sprBullet);

  sprBullet = SDL_LoadBMP("assets/images/bullet5.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBullet[4] = SDL_CreateTextureFromSurface(renderer, sprBullet);
  SDL_FreeSurface(sprBullet);

  sprBullet = SDL_LoadBMP("assets/images/bullet6.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBullet[5] = SDL_CreateTextureFromSurface(renderer, sprBullet);
  SDL_FreeSurface(sprBullet);

  sprBullet = SDL_LoadBMP("assets/images/bullet7.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBulletEnemy = SDL_CreateTextureFromSurface(renderer, sprBullet);

/*
  sprPowerup = SDL_LoadBMP("assets/images/powerup_alpha.bmp");
  SDL_SetColorKey(sprPowerup, SDL_TRUE, SDL_MapRGB(sprPowerup->format, 255, 0, 255));
  imgPowerupAlpha = SDL_CreateTextureFromSurface(renderer, sprPowerup);
*/

for (i = 0; i < NUM_POWERUPS * 2; i++) {
	sprintf(strFile, "assets/images/powerup%d.bmp", (i + 1));
  spr = SDL_LoadBMP(strFile);
  SDL_SetColorKey(spr, SDL_TRUE, SDL_MapRGB(spr->format, 255, 0, 255));
  imgPowerup[i] = SDL_CreateTextureFromSurface(renderer, spr);
  SDL_FreeSurface(spr);
	
}

for (i = 0; i < NUM_SHIP_POWERUPS; i++) {
	sprintf(strFile, "assets/images/ship_powerup%d.bmp", (i + 1));
  spr = SDL_LoadBMP(strFile);
  SDL_SetColorKey(spr, SDL_TRUE, SDL_MapRGB(spr->format, 255, 0, 255));
  imgShipPowerup[i] = SDL_CreateTextureFromSurface(renderer, spr);
  SDL_FreeSurface(spr);
	
}

  sprExplosion = SDL_LoadBMP("assets/images/explosion_l2_1.bmp");
  SDL_SetColorKey(sprExplosion, SDL_TRUE, SDL_MapRGB(sprExplosion->format, 255, 0, 255));
  imgExplosion_L2_00 = SDL_CreateTextureFromSurface(renderer, sprExplosion);

  sprExplosion = SDL_LoadBMP("assets/images/explosion_l2_2.bmp");
  SDL_SetColorKey(sprExplosion, SDL_TRUE, SDL_MapRGB(sprExplosion->format, 255, 0, 255));
  imgExplosion_L2_01 = SDL_CreateTextureFromSurface(renderer, sprExplosion);
  
//int i;
//char strFile[32];

for (i = 0; i < 16; i++) {
	sprintf(strFile, "assets/images/button_fire%d.bmp", (i + 1));
  sprFireButton = SDL_LoadBMP(strFile);
  SDL_SetColorKey(sprFireButton, SDL_TRUE, SDL_MapRGB(sprFireButton->format, 255, 0, 255));
  imgFireButton[i] = SDL_CreateTextureFromSurface(renderer, sprFireButton);
  SDL_FreeSurface(sprFireButton);
	
}

for (i = 0; i < 5; i++) {
	sprintf(strFile, "assets/images/health_unit%d.bmp", (i + 1));
  spr = SDL_LoadBMP(strFile);
  SDL_SetColorKey(spr, SDL_TRUE, SDL_MapRGB(spr->format, 255, 0, 255));
  imgHealthUnit[i] = SDL_CreateTextureFromSurface(renderer, spr);
  SDL_FreeSurface(spr);
	
}

  
  printf("created textures\n");


  SDL_FreeSurface(sprBackground);
//  SDL_FreeSurface(sprPowerup);


//handle loading fonts
  if (TTF_Init() == -1) {
    exit(1);
  }
  fontDefault = TTF_OpenFont("assets/SudburyBasin-Regular.ttf", 20);
  fontLarge = TTF_OpenFont("assets/SudburyBasin-Regular.ttf", 64);
  
  if (!fontDefault || !fontLarge) {
	  printf("Errors loading font\n");
  }

 
//handle loading sounds
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
  soundShoot = Mix_LoadWAV("assets/audio/shoot.wav");
  soundEnemyDead = Mix_LoadWAV("assets/audio/enemy_dead.wav");
  soundEnemyShoot = Mix_LoadWAV("assets/audio/enemy_shoot.wav");
  soundEnemyHit = Mix_LoadWAV("assets/audio/enemy_hit.wav");
  soundEnemyShield = Mix_LoadWAV("assets/audio/enemy_shield.wav");
  soundShipHit = Mix_LoadWAV("assets/audio/ship_hit.wav");
  soundShipDead = Mix_LoadWAV("assets/audio/ship_dead.wav");
  soundPowerup = Mix_LoadWAV("assets/audio/powerup.wav");
  soundWeaponSelect = Mix_LoadWAV("assets/audio/weapon_select.wav");

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
  Mix_FreeChunk(soundEnemyHit);
  Mix_FreeChunk(soundEnemyShield);
  Mix_FreeChunk(soundShipHit);
  Mix_FreeChunk(soundShipDead);
  Mix_FreeChunk(soundPowerup);
  Mix_FreeChunk(soundWeaponSelect);

  Mix_FreeMusic(musicGame);
  Mix_FreeMusic(musicTitle);
  

  Mix_CloseAudio();


for (i = 0; i < NUM_WEAPONS; i++) {
  SDL_DestroyTexture(imgShip[i]);
}  

/*  
  SDL_DestroyTexture(imgShip[0]);
  SDL_DestroyTexture(imgShip[1]);
  SDL_DestroyTexture(imgShip[2]);
  SDL_DestroyTexture(imgShip[3]);
  SDL_DestroyTexture(imgShip[4]);
  */
  
  
for (i = 0; i < NUM_WORLDS * 2; i++) {
  SDL_DestroyTexture(imgBackground[i]);
}  
  
  SDL_DestroyTexture(imgEnemyAlpha_L1_00);
  SDL_DestroyTexture(imgEnemyAlpha_L1_01);
  SDL_DestroyTexture(imgEnemyAlpha_L2_00);
  SDL_DestroyTexture(imgEnemyAlpha_L2_01);
  SDL_DestroyTexture(imgEnemyBravo_L1_00);
  SDL_DestroyTexture(imgEnemyBravo_L1_01);
  SDL_DestroyTexture(imgEnemyBravo_L2_00);
  SDL_DestroyTexture(imgEnemyBravo_L2_01);
  SDL_DestroyTexture(imgEnemyCharlie_L1_00);
  SDL_DestroyTexture(imgEnemyCharlie_L1_01);
  SDL_DestroyTexture(imgEnemyCharlie_L2_00);
  SDL_DestroyTexture(imgEnemyCharlie_L2_01);
  SDL_DestroyTexture(imgEnemyDelta_00);
  SDL_DestroyTexture(imgEnemyDelta_01);
  SDL_DestroyTexture(imgEnemyEcho_00);
  SDL_DestroyTexture(imgEnemyEcho_01);
  SDL_DestroyTexture(imgEnemyEcho_02);
  SDL_DestroyTexture(imgEnemyEcho_03);
  SDL_DestroyTexture(imgExplosion_L2_00);
  SDL_DestroyTexture(imgExplosion_L2_01);
  
  SDL_DestroyTexture(imgBullet[0]);
  SDL_DestroyTexture(imgBullet[1]);
  SDL_DestroyTexture(imgBullet[2]);
  SDL_DestroyTexture(imgBullet[3]);
  SDL_DestroyTexture(imgBullet[4]);
  SDL_DestroyTexture(imgBullet[5]);
  SDL_DestroyTexture(imgScoreText);
  SDL_DestroyTexture(imgLevelCompleteText);
  SDL_DestroyTexture(imgLevel);
  SDL_DestroyTexture(imgWeaponText);
    SDL_DestroyTexture(imgGameTimeText);
    SDL_DestroyTexture(imgTitleStartText);
    SDL_DestroyTexture(imgTitleContinueText);
    SDL_DestroyTexture(imgTitleQuitText);
	
for (i = 0; i < 16; i++) {
	SDL_DestroyTexture(imgFireButton[i]);
}

for (i = 0; i < 3; i++) {
	SDL_DestroyTexture(imgFireButtonText[i]);
}

for (i = 0; i < NUM_WORLDS; i++) {
	SDL_DestroyTexture(imgWorldSelectWorldText[i]);
}

for (i = 0; i < 5; i++) {
	SDL_DestroyTexture(imgHealthUnit[i]);
}

for (i = 0; i < NUM_POWERUPS * 2; i++) {
	SDL_DestroyTexture(imgPowerup[i]);
}

for (i = 0; i < NUM_SHIP_POWERUPS; i++) {
	SDL_DestroyTexture(imgShipPowerup[i]);
}

  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
