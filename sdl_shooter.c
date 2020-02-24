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
SDL_Texture *imgEnemyDelta_L1_00;
SDL_Texture *imgEnemyDelta_L1_01;
SDL_Texture *imgEnemyEcho_L1_00;
SDL_Texture *imgEnemyEcho_L1_01;
SDL_Texture *imgEnemyEcho_L1_02;
SDL_Texture *imgEnemyEcho_L1_03;
SDL_Texture *imgEnemyFoxtrot_L1_00;
SDL_Texture *imgEnemyFoxtrot_L1_01;
SDL_Texture *imgEnemyFoxtrot_L2_00;
SDL_Texture *imgEnemyFoxtrot_L2_01;
SDL_Texture *imgEnemyGolf_L1_00;
SDL_Texture *imgEnemyGolf_L1_01;
SDL_Texture *imgEnemyGolf_L1_02;
SDL_Texture *imgEnemyGolf_L1_03;
SDL_Texture *imgEnemyGolf_L1_04;
SDL_Texture *imgEnemyGolf_L1_05;
SDL_Texture *imgEnemyGolf_L2_00;
SDL_Texture *imgEnemyGolf_L2_01;
SDL_Texture *imgEnemyGolf_L2_02;
SDL_Texture *imgEnemyGolf_L2_03;
SDL_Texture *imgEnemyGolf_L2_04;
SDL_Texture *imgEnemyGolf_L2_05;
SDL_Texture *imgEnemyHotel_L1_00;
SDL_Texture *imgEnemyHotel_L1_01;
SDL_Texture *imgEnemyHotel_L2_00;
SDL_Texture *imgEnemyHotel_L2_01;
SDL_Texture *imgEnemyIndia_L1_00;
SDL_Texture *imgEnemyIndia_L1_01;
SDL_Texture *imgEnemyIndia_L2_00;
SDL_Texture *imgEnemyIndia_L2_01;
SDL_Texture *imgEnemyIndia_L3_00;
SDL_Texture *imgEnemyIndia_L3_01;
SDL_Texture *imgEnemyJuliett_L1_00;
SDL_Texture *imgEnemyJuliett_L1_01;
SDL_Texture *imgEnemyJuliett_L2_00;
SDL_Texture *imgEnemyJuliett_L2_01;
SDL_Surface *surfaceEnemyKilo_L1_00;
SDL_Surface *surfaceEnemyKilo_L1_01;
SDL_Surface *surfaceEnemyKilo_L2_00;
SDL_Surface *surfaceEnemyKilo_L2_01;
SDL_Texture *imgEnemyKiloZero;
SDL_Texture *imgEnemyKiloOne;
SDL_Texture *imgEnemyLima_L1_00;
SDL_Texture *imgEnemyLima_L1_01;
SDL_Texture *imgEnemyLima_L1_02;
SDL_Texture *imgEnemyLima_L1_03;
SDL_Texture *imgEnemyLima_L2_00;
SDL_Texture *imgEnemyLima_L2_01;
SDL_Texture *imgEnemyLima_L2_02;
SDL_Texture *imgEnemyLima_L2_03;
SDL_Texture *imgEnemyMike_L1_00;
SDL_Texture *imgEnemyMike_L1_01;
SDL_Texture *imgEnemyMike_L1_02;
SDL_Texture *imgEnemyMike_L1_03;




SDL_Texture *imgEnemyWarp;
SDL_Texture *imgExplosion_L2_00;
SDL_Texture *imgExplosion_L2_01;
SDL_Texture *imgFireButton[NUM_WEAPONS * 3];
SDL_Texture *imgFireButtonText[3];
SDL_Texture *imgHealthUnit[5];
SDL_Texture *imgButtonWeaponSwitch;
SDL_Texture *imgButtonWeaponSwitchText;



SDL_Texture *imgBackground[NUM_WORLDS * 2];
SDL_Texture *imgBullet[NUM_WEAPONS];
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
SDL_Texture *imgStatsText;
SDL_Texture *imgBonusText[3];

SDL_Texture *imgWorldSelectText;
SDL_Texture *imgWorldSelectSelectedText;
SDL_Texture *imgWorldSelectWorldText[NUM_WORLDS];
SDL_Texture *imgWorldSelectTotalTimeText;


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

Mix_Music *musicGame[NUM_WORLDS]; 
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

void handleCommandLineParameter(char *strParameter) {
	if (strcmp(strParameter, "-nomusic") == 0) {
		Mix_VolumeMusic(0);
	} else if (strcmp(strParameter, "-nosound") == 0) {
		Mix_Volume(-1, 0);
	}
}

SDL_Texture *generateTexture(char *strFile) {
	SDL_Surface *spr;
	SDL_Texture *img;
  spr = SDL_LoadBMP(strFile);
  if (spr == NULL) {
	  printf("Error on loading file %s: %s\n", strFile, SDL_GetError());
  }
  SDL_SetColorKey(spr, SDL_TRUE, SDL_MapRGB(spr->format, 255, 0, 255));
  img = SDL_CreateTextureFromSurface(renderer, spr);
  SDL_FreeSurface(spr);
  
  return img;

}

SDL_Surface *generateSurface(char *strFile) {
	SDL_Surface *spr;
  spr = SDL_LoadBMP(strFile);
  if (spr == NULL) {
	  printf("Error on loading file %s: %s\n", strFile, SDL_GetError());
  }
  SDL_SetColorKey(spr, SDL_TRUE, SDL_MapRGB(spr->format, 255, 0, 255));
  
  return spr;

}



int main(int argc, char* args[]) {

  //seed randomizer
  srand(time(NULL));

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



//handle loading fonts
  if (TTF_Init() == -1) {
    exit(1);
  }
  fontDefault = TTF_OpenFont("assets/SudburyBasin-Regular.ttf", 20);
  fontLarge = TTF_OpenFont("assets/SudburyBasin-Regular.ttf", 64);
  
  if (!fontDefault || !fontLarge) {
	  printf("Errors loading font\n");
  }



  //load media  


int i;
char strFile[64];


printf("start load ship images\n");
for (i = 0; i < NUM_WEAPONS; i++) {
	sprintf(strFile, "assets/images/ship%d.bmp", (i + 1));
  imgShip[i] = generateTexture(strFile);
	
}


  

for (i = 0; i < NUM_WORLDS * 2; i++) {
	sprintf(strFile, "assets/images/background%d.bmp", (i + 1));
  
  imgBackground[i] = generateTexture(strFile);


}


  imgEnemyAlpha_L1_00 = generateTexture("assets/images/enemy_alpha_l1_1.bmp");
  imgEnemyAlpha_L1_01 = generateTexture("assets/images/enemy_alpha_l1_2.bmp");
  imgEnemyAlpha_L2_00 = generateTexture("assets/images/enemy_alpha_l2_1.bmp");
  imgEnemyAlpha_L2_01 = generateTexture("assets/images/enemy_alpha_l2_2.bmp");

  imgEnemyBravo_L1_00 = generateTexture("assets/images/enemy_bravo_l1_1.bmp");
  imgEnemyBravo_L1_01 = generateTexture("assets/images/enemy_bravo_l1_2.bmp");
  imgEnemyBravo_L2_00 = generateTexture("assets/images/enemy_bravo_l2_1.bmp");
  imgEnemyBravo_L2_01 = generateTexture("assets/images/enemy_bravo_l2_2.bmp");


  imgEnemyCharlie_L1_00 = generateTexture("assets/images/enemy_charlie_l1_1.bmp");
  imgEnemyCharlie_L1_01 = generateTexture("assets/images/enemy_charlie_l1_2.bmp");
  imgEnemyCharlie_L2_00 = generateTexture("assets/images/enemy_charlie_l2_1.bmp");
  imgEnemyCharlie_L2_01 = generateTexture("assets/images/enemy_charlie_l2_2.bmp");


  imgEnemyDelta_L1_00 = generateTexture("assets/images/enemy_delta_l1_1.bmp");
  imgEnemyDelta_L1_01 = generateTexture("assets/images/enemy_delta_l1_2.bmp");

  imgEnemyEcho_L1_00 = generateTexture("assets/images/enemy_echo_l1_1.bmp");
  imgEnemyEcho_L1_01 = generateTexture("assets/images/enemy_echo_l1_2.bmp");
  imgEnemyEcho_L1_02 = generateTexture("assets/images/enemy_echo_l1_3.bmp");
  imgEnemyEcho_L1_03 = generateTexture("assets/images/enemy_echo_l1_4.bmp");


  imgEnemyFoxtrot_L1_00 = generateTexture("assets/images/enemy_foxtrot_l1_1.bmp");
  imgEnemyFoxtrot_L1_01 = generateTexture("assets/images/enemy_foxtrot_l1_2.bmp");
  imgEnemyFoxtrot_L2_00 = generateTexture("assets/images/enemy_foxtrot_l2_1.bmp");
  imgEnemyFoxtrot_L2_01 = generateTexture("assets/images/enemy_foxtrot_l2_2.bmp");


  imgEnemyGolf_L1_00 = generateTexture("assets/images/enemy_golf_l1_1.bmp");
  imgEnemyGolf_L1_01 = generateTexture("assets/images/enemy_golf_l1_2.bmp");
  imgEnemyGolf_L1_02 = generateTexture("assets/images/enemy_golf_l1_3.bmp");
  imgEnemyGolf_L1_03 = generateTexture("assets/images/enemy_golf_l1_4.bmp");
  imgEnemyGolf_L1_04 = generateTexture("assets/images/enemy_golf_l1_5.bmp");
  imgEnemyGolf_L1_05 = generateTexture("assets/images/enemy_golf_l1_6.bmp");

  imgEnemyGolf_L2_00 = generateTexture("assets/images/enemy_golf_l2_1.bmp");
  imgEnemyGolf_L2_01 = generateTexture("assets/images/enemy_golf_l2_2.bmp");
  imgEnemyGolf_L2_02 = generateTexture("assets/images/enemy_golf_l2_3.bmp");
  imgEnemyGolf_L2_03 = generateTexture("assets/images/enemy_golf_l2_4.bmp");
  imgEnemyGolf_L2_04 = generateTexture("assets/images/enemy_golf_l2_5.bmp");
  imgEnemyGolf_L2_05 = generateTexture("assets/images/enemy_golf_l2_6.bmp");

  imgEnemyHotel_L1_00 = generateTexture("assets/images/enemy_hotel_l1_1.bmp");
  imgEnemyHotel_L1_01 = generateTexture("assets/images/enemy_hotel_l1_2.bmp");
  imgEnemyHotel_L2_00 = generateTexture("assets/images/enemy_hotel_l2_1.bmp");
  imgEnemyHotel_L2_01 = generateTexture("assets/images/enemy_hotel_l2_2.bmp");
    
  imgEnemyIndia_L1_00 = generateTexture("assets/images/enemy_india_l1_1.bmp");
  imgEnemyIndia_L1_01 = generateTexture("assets/images/enemy_india_l1_2.bmp");
  imgEnemyIndia_L2_00 = generateTexture("assets/images/enemy_india_l2_1.bmp");
  imgEnemyIndia_L2_01 = generateTexture("assets/images/enemy_india_l2_2.bmp");
  imgEnemyIndia_L3_00 = generateTexture("assets/images/enemy_india_l3_1.bmp");
  imgEnemyIndia_L3_01 = generateTexture("assets/images/enemy_india_l3_2.bmp");

  imgEnemyJuliett_L1_00 = generateTexture("assets/images/enemy_juliett_l1_1.bmp");
  imgEnemyJuliett_L1_01 = generateTexture("assets/images/enemy_juliett_l1_2.bmp");
  imgEnemyJuliett_L2_00 = generateTexture("assets/images/enemy_juliett_l2_1.bmp");
  imgEnemyJuliett_L2_01 = generateTexture("assets/images/enemy_juliett_l2_2.bmp");
    
    surfaceEnemyKilo_L1_00 = generateSurface("assets/images/enemy_kilo_l1_1.bmp");
    surfaceEnemyKilo_L1_01 = generateSurface("assets/images/enemy_kilo_l1_2.bmp");
    surfaceEnemyKilo_L2_00 = generateSurface("assets/images/enemy_kilo_l2_1.bmp");
    surfaceEnemyKilo_L2_01 = generateSurface("assets/images/enemy_kilo_l2_2.bmp");
    SDL_Color colorGreenText = {64, 255, 64};
    generateTextTexture(&imgEnemyKiloZero, "0", colorGreenText, fontDefault);
    generateTextTexture(&imgEnemyKiloOne, "1", colorGreenText, fontDefault);
    
    imgEnemyLima_L1_00 = generateTexture("assets/images/enemy_lima_l1_1.bmp");
    imgEnemyLima_L1_01 = generateTexture("assets/images/enemy_lima_l1_2.bmp");
    imgEnemyLima_L1_02 = generateTexture("assets/images/enemy_lima_l1_3.bmp");
    imgEnemyLima_L1_03 = generateTexture("assets/images/enemy_lima_l1_4.bmp");
    imgEnemyLima_L2_00 = generateTexture("assets/images/enemy_lima_l2_1.bmp");
    imgEnemyLima_L2_01 = generateTexture("assets/images/enemy_lima_l2_2.bmp");
    imgEnemyLima_L2_02 = generateTexture("assets/images/enemy_lima_l2_3.bmp");
    imgEnemyLima_L2_03 = generateTexture("assets/images/enemy_lima_l2_4.bmp");

    imgEnemyMike_L1_00 = generateTexture("assets/images/enemy_mike_l1_1.bmp");
    imgEnemyMike_L1_01 = generateTexture("assets/images/enemy_mike_l1_2.bmp");
    imgEnemyMike_L1_02 = generateTexture("assets/images/enemy_mike_l1_3.bmp");
    imgEnemyMike_L1_03 = generateTexture("assets/images/enemy_mike_l1_4.bmp");



	for (i = 0; i < NUM_WEAPONS; i++) {
			sprintf(strFile, "assets/images/bullet%d.bmp", (i + 1));

		imgBullet[i] = generateTexture(strFile);
		
	}

	sprintf(strFile, "assets/images/bullet%d.bmp", NUM_WEAPONS + 1);
	imgBulletEnemy = generateTexture(strFile);
  

for (i = 0; i < NUM_POWERUPS * 2; i++) {
	sprintf(strFile, "assets/images/powerup%d.bmp", (i + 1));
  		imgPowerup[i] = generateTexture(strFile);

	
}

for (i = 0; i < NUM_SHIP_POWERUPS; i++) {
	sprintf(strFile, "assets/images/ship_powerup%d.bmp", (i + 1));
		imgShipPowerup[i] = generateTexture(strFile);

	
}


	imgExplosion_L2_00 = generateTexture("assets/images/explosion_l2_1.bmp");
	imgExplosion_L2_01 = generateTexture("assets/images/explosion_l2_2.bmp");

  

for (i = 0; i < (NUM_WEAPONS * 3); i++) {
	sprintf(strFile, "assets/images/button_fire%d.bmp", (i + 1));
//	printf("loading i: %d, %s\n", i, strFile);
  	imgFireButton[i] = generateTexture(strFile);

	
}

for (i = 0; i < 5; i++) {
	sprintf(strFile, "assets/images/health_unit%d.bmp", (i + 1));
   	imgHealthUnit[i] = generateTexture(strFile);

	
}

  	imgButtonWeaponSwitch = generateTexture("assets/images/button_weapon_switch.bmp");
	imgEnemyWarp = generateTexture("assets/images/enemy_warp.bmp");


  



 
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

  for (i = 0; i < NUM_WORLDS; i++) {
	sprintf(strFile, "assets/audio/sdl-shooter-level-%d.wav", (i + 1));
	musicGame[i] = Mix_LoadMUS(strFile);
  }
  musicTitle = Mix_LoadMUS("assets/audio/sdl-shooter-title.wav");

		
	for (i = 0; i < argc; i++) {
		printf("args[%d]: %s\n", i, args[i]);
		handleCommandLineParameter(args[i]);
	}



  start();

  iTime = SDL_GetTicks();
  
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
      } else if ((theEvent.type == SDL_KEYDOWN || theEvent.type == SDL_KEYUP) && (theEvent.key.repeat == 0)) {
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
  
//  printf("Stopped looping\n");

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

//  Mix_FreeMusic(musicGame);
  for (i = 0; i < NUM_WORLDS; i++) {
	Mix_FreeMusic(musicGame[i]);
  }

  Mix_FreeMusic(musicTitle);
  

  Mix_CloseAudio();


for (i = 0; i < NUM_WEAPONS; i++) {
  SDL_DestroyTexture(imgShip[i]);
}  

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
  SDL_DestroyTexture(imgEnemyDelta_L1_00);
  SDL_DestroyTexture(imgEnemyDelta_L1_01);
  SDL_DestroyTexture(imgEnemyEcho_L1_00);
  SDL_DestroyTexture(imgEnemyEcho_L1_01);
  SDL_DestroyTexture(imgEnemyEcho_L1_02);
  SDL_DestroyTexture(imgEnemyEcho_L1_03);
  SDL_DestroyTexture(imgEnemyFoxtrot_L1_00);
  SDL_DestroyTexture(imgEnemyFoxtrot_L1_01);
  SDL_DestroyTexture(imgEnemyFoxtrot_L2_00);
  SDL_DestroyTexture(imgEnemyFoxtrot_L2_01);
  SDL_DestroyTexture(imgEnemyGolf_L1_00);
  SDL_DestroyTexture(imgEnemyGolf_L1_01);
  SDL_DestroyTexture(imgEnemyGolf_L1_02);
  SDL_DestroyTexture(imgEnemyGolf_L1_03);
  SDL_DestroyTexture(imgEnemyGolf_L1_04);
  SDL_DestroyTexture(imgEnemyGolf_L1_05);
  SDL_DestroyTexture(imgEnemyGolf_L2_00);
  SDL_DestroyTexture(imgEnemyGolf_L2_01);
  SDL_DestroyTexture(imgEnemyGolf_L2_02);
  SDL_DestroyTexture(imgEnemyGolf_L2_03);
  SDL_DestroyTexture(imgEnemyGolf_L2_04);
  SDL_DestroyTexture(imgEnemyGolf_L2_05);
  SDL_DestroyTexture(imgEnemyHotel_L1_00);
  SDL_DestroyTexture(imgEnemyHotel_L1_01);
  SDL_DestroyTexture(imgEnemyHotel_L2_00);
  SDL_DestroyTexture(imgEnemyHotel_L2_01);
  SDL_DestroyTexture(imgEnemyIndia_L1_00);
  SDL_DestroyTexture(imgEnemyIndia_L1_01);
  SDL_DestroyTexture(imgEnemyIndia_L2_00);
  SDL_DestroyTexture(imgEnemyIndia_L2_01);
  SDL_DestroyTexture(imgEnemyIndia_L3_00);
  SDL_DestroyTexture(imgEnemyIndia_L3_01);
  SDL_DestroyTexture(imgEnemyJuliett_L1_00);
  SDL_DestroyTexture(imgEnemyJuliett_L1_01);
  SDL_DestroyTexture(imgEnemyJuliett_L2_00);
  SDL_DestroyTexture(imgEnemyJuliett_L2_01);
    SDL_FreeSurface(surfaceEnemyKilo_L1_00);
    SDL_FreeSurface(surfaceEnemyKilo_L1_01);
    SDL_FreeSurface(surfaceEnemyKilo_L2_00);
    SDL_FreeSurface(surfaceEnemyKilo_L2_01);
    SDL_DestroyTexture(imgEnemyKiloZero);
    SDL_DestroyTexture(imgEnemyKiloOne);
    SDL_DestroyTexture(imgEnemyLima_L1_00);
    SDL_DestroyTexture(imgEnemyLima_L1_01);
    SDL_DestroyTexture(imgEnemyLima_L1_02);
    SDL_DestroyTexture(imgEnemyLima_L1_03);
    SDL_DestroyTexture(imgEnemyLima_L2_00);
    SDL_DestroyTexture(imgEnemyLima_L2_01);
    SDL_DestroyTexture(imgEnemyLima_L2_02);
    SDL_DestroyTexture(imgEnemyLima_L2_03);
    SDL_DestroyTexture(imgEnemyMike_L1_00);
    SDL_DestroyTexture(imgEnemyMike_L1_01);
    SDL_DestroyTexture(imgEnemyMike_L1_02);
    SDL_DestroyTexture(imgEnemyMike_L1_03);


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

    SDL_DestroyTexture(imgStatsText);

	
for (i = 0; i < (NUM_WEAPONS * 3); i++) {
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

for (i = 0; i < 3; i++) {
	SDL_DestroyTexture(imgBonusText[i]);
}
    
    SDL_DestroyTexture(imgButtonWeaponSwitch);
    SDL_DestroyTexture(imgButtonWeaponSwitchText);
    SDL_DestroyTexture(imgEnemyWarp);


  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
