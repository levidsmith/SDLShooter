//2019 Levi D. Smith - levidsmith.com
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "enemy.h"
#include "ship.h"
#include "level_reader.h"
#include "globals.h"


#define MAX_ENEMIES 15

//SDL variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


SDL_Surface* screenSurface = NULL;

SDL_Texture *imgShip;
SDL_Texture *imgEnemy_00;
SDL_Texture *imgEnemy_01;
SDL_Texture *imgBackground;
SDL_Texture *imgBullet;
SDL_Texture *imgScoreText;
SDL_Texture *imgLevelCompleteText;
SDL_Texture *imgGameOverText;


TTF_Font *fontDefault;
TTF_Font *fontLarge;

Mix_Chunk *soundShoot;
Mix_Chunk *soundEnemyDead;

Mix_Music *musicLevel; 

//VARIABLES
//SDL_Rect shipPosition;
SDL_Rect pos;
SDL_Rect posText;

char strFPS[64];
Uint32 iTime;
Uint32 iDelay;
int iFrames;
int iFPS;



struct Ship *ship;


struct Enemy enemyList[MAX_ENEMIES];

struct Bullet {
  int x;
  int y;
  int width;
  int height;
  int isAlive;
};
struct Bullet bullet;

struct EnemyBullet {
  float x;
  float y;
  int width;
  int height;
  int isAlive;  
};

int iKeepLooping = TRUE;
int iBackgroundOffset;
int iLevelComplete = FALSE;
int iGameOver = FALSE;

int iScore;

//FUNCTION PROTOTYPES
void handleInput(int, int);
void shoot();
void checkCollisions();
//void addEnemy(struct Enemy *, int, int);
void checkLevelComplete();
void updateScoreText();


//FUNCTIONS

void start() {
	
  ship = malloc(sizeof(struct Ship));
  init_ship(ship);


  int iSpacing = 128;
  int x_offset = (SCREEN_WIDTH - (5 * iSpacing)) / 2;

  printf("call read_level");
  read_level("level_00.txt");


  bullet.isAlive = FALSE;
  bullet.width = 16;
  bullet.height = 16;

  iScore = 0;
  updateScoreText();

  Mix_VolumeMusic(MIX_MAX_VOLUME * 0.2);
  Mix_PlayMusic(musicLevel, -1);
  
}

void update() {
  int i;

   update_ship(ship);

  //Update the enemies
  for (i = 0; i < MAX_ENEMIES; i++) {
    update_enemy(&enemyList[i]);
  }


  //update bullet
  if (bullet.isAlive) {
    bullet.y -= (5 * UNIT_SIZE * DELTA_TIME);
    if (bullet.y < 0) {
      bullet.isAlive = FALSE;
    }
  }
  

  //update background
  iBackgroundOffset += 5 * UNIT_SIZE * DELTA_TIME;
  if (iBackgroundOffset > 255) {
    iBackgroundOffset -= 256;
  }


  checkCollisions();
}

void checkCollisions() {

  int i;

  //Update the enemies
  for (i = 0; i < MAX_ENEMIES; i++) {
    if ( (enemyList[i].isAlive) && (bullet.isAlive) && 
         ((bullet.x + bullet.width / 2) >= enemyList[i].x && (bullet.x + bullet.width / 2) < enemyList[i].x + enemyList[i].width) &&
         ((bullet.y + bullet.height / 2) >= enemyList[i].y && (bullet.y + bullet.height / 2) < enemyList[i].y + enemyList[i].height) ) {
      bullet.isAlive = FALSE;
      enemyList[i].isAlive = FALSE;
      iScore += 100;
	  updateScoreText();
      checkLevelComplete();
      Mix_PlayChannel(-1, soundEnemyDead, 0);
    } 

    if ( (enemyList[i].isAlive) &&  (ship->isAlive) &&
         (ship->x >= enemyList[i].x && ship->x < enemyList[i].x + enemyList[i].width) &&
         (ship->y >= enemyList[i].y && ship->y < enemyList[i].y + enemyList[i].height) ) {
      ship->isAlive = FALSE;
      iGameOver = TRUE;
    } 

  }

}

void checkLevelComplete() {
  int iComplete = TRUE;

  int i;
  for (i = 0; i < MAX_ENEMIES; i++) {
    if  (enemyList[i].isAlive)  {
      iComplete = FALSE;
    }
  }

  iLevelComplete = iComplete;

}


void handleInput(int iType, int iKey) {
  float fSpeed = UNIT_SIZE * 5;

  if (iType == SDL_KEYDOWN) {
    if (iKey == SDLK_UP) {
      ship->vel_y = -fSpeed;
    } else if (iKey == SDLK_DOWN) {
      ship->vel_y = fSpeed;
    } else if (iKey == SDLK_LEFT) {
      ship->vel_x = -fSpeed;
    } else if (iKey == SDLK_RIGHT) {
      ship->vel_x = fSpeed;
    } else if (iKey == SDLK_SPACE) {
      shoot(); 
    } else if (iKey == SDLK_q || iKey == SDLK_ESCAPE) {
      iKeepLooping = FALSE;
    } else if (iKey == SDLK_m || iKey == SDLK_ESCAPE) {
      Mix_VolumeMusic(0);
    }  
  }


  if (iType == SDL_KEYUP) {
    if (iKey == SDLK_UP && ship->vel_y < 0) {
      ship->vel_y = 0;
    } else if (iKey == SDLK_DOWN && ship->vel_y > 0) {
      ship->vel_y = 0;
    } else if (iKey == SDLK_LEFT && ship->vel_x < 0) {
      ship->vel_x = 0;
    } else if (iKey == SDLK_RIGHT && ship->vel_x > 0) {
      ship->vel_x = 0;
    }  
  }


}

/*
void draw() {
	
	
//Draw the background
  int i, j;
  for (i = -1; i < (SCREEN_HEIGHT / 256) + 1; i++) {
    for (j = 0; j < SCREEN_WIDTH / 256; j++) {
      pos.x = j * 256;
      pos.y = i * 256 + iBackgroundOffset;
      SDL_BlitSurface(sprBackground, NULL, screenSurface, &pos);
    }
  }

//Draw the ship
  draw_ship(ship);

//Draw the bullet
  if (bullet.isAlive) {
    pos.x = bullet.x;
    pos.y = bullet.y;
    SDL_BlitSurface(sprBullet, NULL, screenSurface, &pos);
  }
 
//Draw the enemies
  for (i = 0; i < MAX_ENEMIES; i++) {
    draw_enemy(&enemyList[i]);
  }


//Draw the score text
  SDL_Color colorText;
  colorText.r = 255;
  colorText.g = 255;
  colorText.b = 0;

  char strScore[64];
  sprintf(strScore, "Score: %d", iScore);
  SDL_Surface *sprText = TTF_RenderText_Solid(fontDefault, strScore, colorText); 

  pos.x = 100;
  pos.y = 100;
  SDL_BlitSurface(sprText, NULL, screenSurface, &pos);
  SDL_FreeSurface(sprText); 

//draw FPS text
    colorText.r = 255;
    colorText.g = 255;
    colorText.b = 255;
    pos.x = 1000;
    pos.y = 32;
    sprText = TTF_RenderText_Solid(fontDefault, strFPS, colorText); 
    SDL_BlitSurface(sprText, NULL, screenSurface, &pos);
    SDL_FreeSurface(sprText); 


//Draw the level complete text
  if (iLevelComplete) {
    colorText.r = 0;
    colorText.g = 0;
    colorText.b = 255;
    pos.x = 320;
    pos.y = 300;
    sprText = TTF_RenderText_Solid(fontLarge, "LEVEL COMPLETE", colorText); 
    SDL_BlitSurface(sprText, NULL, screenSurface, &pos);
    SDL_FreeSurface(sprText); 
  }

//Draw Game Over Text
  if (iGameOver) {
    colorText.r = 0;
    colorText.g = 0;
    colorText.b = 255;
    pos.x = 320;
    pos.y = 300;
    sprText = TTF_RenderText_Solid(fontLarge, "GAME OVER", colorText); 
    SDL_BlitSurface(sprText, NULL, screenSurface, &pos);
    SDL_FreeSurface(sprText); 
  }



  SDL_UpdateWindowSurface( window);
}

*/

void draw() {
  //Draw the background
  int i, j;
  for (i = -1; i < (SCREEN_HEIGHT / 256) + 1; i++) {
    for (j = 0; j < SCREEN_WIDTH / 256; j++) {
      pos.x = j * 256;
      pos.y = i * 256 + iBackgroundOffset;
	  pos.w = 256;
	  pos.h = 256;
//      SDL_BlitSurface(sprBackground, NULL, screenSurface, &pos);
      SDL_RenderCopy(renderer, imgBackground, NULL, &pos);

    }
  }


  //draw the ship
  draw_ship(ship);

//Draw the bullet
  if (bullet.isAlive) {
    pos.x = bullet.x;
    pos.y = bullet.y;
	pos.w = bullet.width;
	pos.h = bullet.height;
      SDL_RenderCopy(renderer, imgBullet, NULL, &pos);
//    SDL_BlitSurface(sprBullet, NULL, screenSurface, &pos);
  }
 
//Draw the enemies
  for (i = 0; i < MAX_ENEMIES; i++) {
    draw_enemy(&enemyList[i]);
  }

//Draw the score text
  pos.x = 100;
  pos.y = 100;
  
  SDL_QueryTexture(imgScoreText, NULL, NULL, &(pos.w), &(pos.h)); 
  SDL_RenderCopy(renderer, imgScoreText, NULL, &pos);

//Draw level complete text
  if (iLevelComplete) {
    pos.x = 320;
    pos.y = 300;
  
    SDL_QueryTexture(imgLevelCompleteText, NULL, NULL, &(pos.w), &(pos.h)); 
    SDL_RenderCopy(renderer, imgLevelCompleteText, NULL, &pos);
  }

//Draw game over text
  if (iGameOver) {
    pos.x = 320;
    pos.y = 300;
  
    SDL_QueryTexture(imgGameOverText, NULL, NULL, &(pos.w), &(pos.h)); 
    SDL_RenderCopy(renderer, imgGameOverText, NULL, &pos);
  }

	

}

void updateScoreText() {
  SDL_Color colorText;
  colorText.r = 255;
  colorText.g = 255;
  colorText.b = 0;

  char strScore[64];
  sprintf(strScore, "Score: %d", iScore);
  SDL_Surface *sprText = TTF_RenderText_Solid(fontDefault, strScore, colorText);
  imgScoreText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 


  colorText.r = 0;
  colorText.g = 0;
  colorText.b = 255;

  sprText = TTF_RenderText_Solid(fontLarge, "LEVEL COMPLETE", colorText);
  imgLevelCompleteText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

  sprText = TTF_RenderText_Solid(fontLarge, "GAME OVER", colorText);
  imgGameOverText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

	
}


void shoot() {
  if (ship->isAlive && !bullet.isAlive) {


    bullet.isAlive = TRUE;
    bullet.x = ship->x + ((ship->width - bullet.width) / 2);
    bullet.y = ship->y;
    Mix_PlayChannel(-1, soundShoot, 0);
  }
}

/*
void addEnemy(struct Enemy *e, int init_x, int init_y) {
  e->x = init_x;
  e->y = init_y;
  e->vel_x = 0.2;
  e->vel_y = 0;
  e->width = 64;
  e->height = 64;
  e->fChangeMovementCountdown = 60 * 10;
  e->iType = 0;
  e->isAlive = TRUE;
}
*/



int main(int argc, char* args[]) {
//  printf("Starting game");


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
  SDL_Surface* sprEnemy_00;
  SDL_Surface* sprEnemy_01;
  SDL_Surface* sprBackground;
  SDL_Surface* sprBullet;

  
  sprShip = SDL_LoadBMP("assets/images/ship.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  imgShip = SDL_CreateTextureFromSurface(renderer, sprShip);
  
  sprBackground = SDL_LoadBMP("assets/images/background.bmp");
  imgBackground = SDL_CreateTextureFromSurface(renderer, sprBackground);

  sprEnemy_00 = SDL_LoadBMP("assets/images/enemy.bmp");
  SDL_SetColorKey(sprEnemy_00, SDL_TRUE, SDL_MapRGB(sprEnemy_00->format, 255, 0, 255));
  imgEnemy_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy_00);

  sprEnemy_01 = SDL_LoadBMP("assets/images/enemy_01_a.bmp");
  SDL_SetColorKey(sprEnemy_01, SDL_TRUE, SDL_MapRGB(sprEnemy_01->format, 255, 0, 255));
  imgEnemy_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy_01);

  sprBullet = SDL_LoadBMP("assets/images/bullet.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBullet = SDL_CreateTextureFromSurface(renderer, sprBullet);

  SDL_FreeSurface(sprShip);
  SDL_FreeSurface(sprEnemy_00);
  SDL_FreeSurface(sprEnemy_01);
  SDL_FreeSurface(sprBackground);
  SDL_FreeSurface(sprBullet);


//handle loading fonts

  if (TTF_Init() == -1) {
    exit(1);
  }
  fontDefault = TTF_OpenFont("SudburyBasin-Regular.ttf", 20);
  fontLarge = TTF_OpenFont("SudburyBasin-Regular.ttf", 64);
  
//handle loading sounds
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
  soundShoot = Mix_LoadWAV("assets/audio/shoot.wav");
  soundEnemyDead = Mix_LoadWAV("assets/audio/enemy_dead.wav");

  musicLevel = Mix_LoadMUS("assets/audio/sdl-shooter-level.wav");

  start();

  iTime = SDL_GetTicks();
  
  SDL_Event e;
  while (iKeepLooping == TRUE) {
    update();

    draw();
	SDL_RenderPresent(renderer);

//    while (SDL_PollEvent(&e) != 0) {
    while (SDL_PollEvent(&e)) {
			printf("SDL_PollEvent\n");
			
	//SDL_PollEvent(&e);

      if (e.type  == SDL_QUIT) {
		printf("Close button pressed\n");
        iKeepLooping = FALSE;
		printf("Stop looping\n");
		break;
		printf("Shouldn't get here\n");
      } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        handleInput(e.type, e.key.keysym.sym);  
      }

    }
	

    //Set target frame rate
//    iDelay = TICK_INTERVAL - (SDL_GetTicks() - iTime);
    iDelay = (1000 * DELTA_TIME) - (SDL_GetTicks() - iTime);
	if (iDelay < 0) {
		iDelay = 0;
	}
    SDL_Delay(iDelay);
	iFPS = 1000 / (SDL_GetTicks() - iTime);
//	sprintf(strFPS, "FPS: %d", iFPS);
	iTime = SDL_GetTicks();
//	printf("iTime: %d iDelay: %d FPS: %d\n", iTime, iDelay, iFPS);
  }
  
  printf("Stopped looping\n");

  TTF_Quit();

  Mix_FreeMusic(musicLevel);

  Mix_CloseAudio();

  
  SDL_DestroyTexture(imgShip);
  SDL_DestroyTexture(imgBackground);
  SDL_DestroyTexture(imgEnemy_00);
  SDL_DestroyTexture(imgEnemy_01);
  SDL_DestroyTexture(imgBullet);
  SDL_DestroyTexture(imgScoreText);
  SDL_DestroyTexture(imgLevelCompleteText);

  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
