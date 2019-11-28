//2019 Levi D. Smith - levidsmith.com
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "enemy.h"
#include "level_reader.h"
#include "globals.h"

//#define SCREEN_WIDTH 1280 
//#define SCREEN_HEIGHT 720 

//#define TRUE 1
//#define FALSE 0

#define MAX_ENEMIES 15

//SDL variables

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* sprShip;
SDL_Surface* sprEnemy_00;
SDL_Surface* sprEnemy_01;
SDL_Surface* sprBackground;
SDL_Surface* sprBullet;

TTF_Font *fontDefault;
TTF_Font *fontLarge;

Mix_Chunk *soundShoot;
Mix_Chunk *soundEnemyDead;

//VARIABLES
SDL_Rect shipPosition;
SDL_Rect pos;

struct Ship {
  float vel_x;
  float vel_y;
  int x;
  int y;
  int width;
  int height;
  int isAlive;
};
struct Ship ship;


/*
struct Enemy {
  int iHealth;
  float x;
  float y;
  int width;
  int height;
  float vel_x;
  float vel_y;
  float fLifetime;
  float fChangeMovementCountdown;
  int isAlive;
};
*/
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

//FUNCTIONS

void start() {

  ship.width = 64;
  ship.height = 64;
  ship.vel_x = 0;
  ship.vel_y = 0;
  ship.x = (SCREEN_WIDTH - ship.width) / 2;
  ship.y = 720 - 128;
  ship.isAlive = TRUE;


  int iSpacing = 128;
  int x_offset = (SCREEN_WIDTH - (5 * iSpacing)) / 2;

  printf("call read_level");
  read_level("level_00.txt");

/*
  struct Enemy *e1;
  e1 = malloc(sizeof(struct Enemy));
  init_enemy(e1, (iSpacing * 0) + x_offset, 64 + (iSpacing * 0));

  struct Enemy *e2;
  e2 = malloc(sizeof(struct Enemy));
  init_enemy(e2, (iSpacing * 1) + x_offset, 64 + (iSpacing * 0));

  struct Enemy *e3;
  e3 = malloc(sizeof(struct Enemy));
  init_enemy(e3, (iSpacing * 2) + x_offset, 64 + (iSpacing * 0));

  struct Enemy *e4;
  e4 = malloc(sizeof(struct Enemy));
  init_enemy(e4, (iSpacing * 3) + x_offset, 64 + (iSpacing * 0));

  struct Enemy *e5;
  e5 = malloc(sizeof(struct Enemy));
  init_enemy(e5, (iSpacing * 4) + x_offset, 64 + (iSpacing * 0));

  struct Enemy *e6;
  e6 = malloc(sizeof(struct Enemy));
  init_enemy(e6, (iSpacing * (0.5 + 0)) + x_offset, 64 + (iSpacing * 0.5));

  struct Enemy *e7;
  e7 = malloc(sizeof(struct Enemy));
  init_enemy(e7, (iSpacing * (0.5 + 1)) + 40 + x_offset, 64 + (iSpacing * 0.5));

  struct Enemy *e8;
  e8 = malloc(sizeof(struct Enemy));
  init_enemy(e8, (iSpacing * (0.5 + 2)) + 40 + x_offset, 64 + (iSpacing * 0.5));

  struct Enemy *e9;
  e9 = malloc(sizeof(struct Enemy));
  init_enemy(e9, (iSpacing * (0.5 + 3)) + 40 + x_offset, 64 + (iSpacing * 0.5));


  struct Enemy *e10;
  e10 = malloc(sizeof(struct Enemy));
  init_enemy(e10, (iSpacing * (1 + 0)) + 80 + x_offset, 64 + (iSpacing * 1));

  struct Enemy *e11;
  e11 = malloc(sizeof(struct Enemy));
  init_enemy(e11, (iSpacing * (1 + 1)) + 80 + x_offset, 64 + (iSpacing * 1));

  struct Enemy *e12;
  e12 = malloc(sizeof(struct Enemy));
  init_enemy(e12, (iSpacing * (1 + 2)) + 80 + x_offset, 64 + (iSpacing * 1));


  struct Enemy *e13;
  e13 = malloc(sizeof(struct Enemy));
  init_enemy(e13, (iSpacing * (1.5 + 0)) + 120 + x_offset, 64 + (iSpacing * 1.5));

  struct Enemy *e14;
  e14 = malloc(sizeof(struct Enemy));
  init_enemy(e14, (iSpacing * (1.5 + 1)) + 120 + x_offset, 64 + (iSpacing * 1.5));


  struct Enemy *e15;
  e15 = malloc(sizeof(struct Enemy));
  init_enemy(e15, (iSpacing * (2 + 0)) + 160 + x_offset, 64 + (iSpacing * 2));


  enemyList[0] = *e1; 
  enemyList[1] = *e2; 
  enemyList[2] = *e3; 
  enemyList[3] = *e4; 
  enemyList[4] = *e5; 
  enemyList[5] = *e6; 
  enemyList[6] = *e7; 
  enemyList[7] = *e8; 
  enemyList[8] = *e9; 
  enemyList[9] = *e10; 
  enemyList[10] = *e11; 
  enemyList[11] = *e12; 
  enemyList[12] = *e13; 
  enemyList[13] = *e14; 
  enemyList[14] = *e15; 
*/


  bullet.isAlive = FALSE;
  bullet.width = 16;
  bullet.height = 16;

  iScore = 0;
}

void update() {
  int i;

  //update ship
  if (ship.isAlive) {
    ship.x += ship.vel_x;
    ship.y += ship.vel_y;

    if (ship.x < 0) {
      ship.x = 0;
    }

    if (ship.y < 0) {
      ship.y = 0;
    }

    if (ship.x > SCREEN_WIDTH - (ship.width)) {
      ship.x = SCREEN_WIDTH - (ship.width);
    }

    if (ship.y > SCREEN_HEIGHT - ship.height) {
      ship.y = SCREEN_HEIGHT - ship.height;
    }
  }

  //Update the enemies
  for (i = 0; i < MAX_ENEMIES; i++) {
/*
    enemyList[i].fLifetime += 0.2;

    enemyList[i].fChangeMovementCountdown = enemyList[i].fChangeMovementCountdown - 1;
    if (enemyList[i].fChangeMovementCountdown <= 0) {
      enemyList[i].vel_x *= -1; 
      enemyList[i].fChangeMovementCountdown = 60 * 20; 
    } 
    enemyList[i].x += enemyList[i].vel_x;
    enemyList[i].y += enemyList[i].vel_y;
*/
    update_enemy(&enemyList[i]);
  }


  //update bullet
  if (bullet.isAlive) {
    bullet.y -= 2;
    if (bullet.y < 0) {
      bullet.isAlive = FALSE;
    }
  }
  

  //update background
  iBackgroundOffset++;
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
      checkLevelComplete();
      Mix_PlayChannel(-1, soundEnemyDead, 0);
    } 

    if ( (enemyList[i].isAlive) &&  (ship.isAlive) &&
         (ship.x >= enemyList[i].x && ship.x < enemyList[i].x + enemyList[i].width) &&
         (ship.y >= enemyList[i].y && ship.y < enemyList[i].y + enemyList[i].height) ) {
      ship.isAlive = FALSE;
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
  float fSpeed = 1;

  if (iType == SDL_KEYDOWN) {
    if (iKey == SDLK_UP) {
      ship.vel_y = -fSpeed;
    } else if (iKey == SDLK_DOWN) {
      ship.vel_y = fSpeed;
    } else if (iKey == SDLK_LEFT) {
      ship.vel_x = -fSpeed;
    } else if (iKey == SDLK_RIGHT) {
      ship.vel_x = fSpeed;
    } else if (iKey == SDLK_SPACE) {
      shoot(); 
    } else if (iKey == SDLK_q || iKey == SDLK_ESCAPE) {
      iKeepLooping = FALSE;
    }  
  }


  if (iType == SDL_KEYUP) {
    if (iKey == SDLK_UP && ship.vel_y < 0) {
      ship.vel_y = 0;
    } else if (iKey == SDLK_DOWN && ship.vel_y > 0) {
      ship.vel_y = 0;
    } else if (iKey == SDLK_LEFT && ship.vel_x < 0) {
      ship.vel_x = 0;
    } else if (iKey == SDLK_RIGHT && ship.vel_x > 0) {
      ship.vel_x = 0;
    }  
  }


}

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
  if (ship.isAlive) {
    shipPosition.x = ship.x;
    shipPosition.y = ship.y;
    SDL_BlitSurface(sprShip, NULL, screenSurface, &shipPosition);
  }

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

void shoot() {
  if (ship.isAlive && !bullet.isAlive) {


    bullet.isAlive = TRUE;
    bullet.x = ship.x + ((ship.width - bullet.width) / 2);
    bullet.y = ship.y;
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
  printf("Starting game");


  if (SDL_Init( SDL_INIT_VIDEO) < 0) {
    printf("Error: %s\n", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow("SDL Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Error: %s\n", SDL_GetError());
    return 1;
  }
    
  screenSurface = SDL_GetWindowSurface(window);

  
  sprShip = SDL_LoadBMP("ship.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  sprBackground = SDL_LoadBMP("background.bmp");

  sprEnemy_00 = SDL_LoadBMP("enemy.bmp");
  SDL_SetColorKey(sprEnemy_00, SDL_TRUE, SDL_MapRGB(sprEnemy_00->format, 255, 0, 255));

  sprEnemy_01 = SDL_LoadBMP("enemy_01_a.bmp");
  SDL_SetColorKey(sprEnemy_01, SDL_TRUE, SDL_MapRGB(sprEnemy_01->format, 255, 0, 255));

  sprBullet = SDL_LoadBMP("bullet.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));

//handle loading fonts

  if (TTF_Init() == -1) {
    exit(1);
  }
  fontDefault = TTF_OpenFont("SudburyBasin-Regular.ttf", 20);
  fontLarge = TTF_OpenFont("SudburyBasin-Regular.ttf", 64);
  
//handle loading sounds
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
  soundShoot = Mix_LoadWAV("shoot.wav");
  soundEnemyDead = Mix_LoadWAV("enemy_dead.wav");

  start();

  SDL_Event e;
  while (iKeepLooping == TRUE) {
    update();
    draw();

    while (SDL_PollEvent(&e) != 0) {
      if (e.type  == SDL_QUIT) {
        iKeepLooping = FALSE;
      } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        handleInput(e.type, e.key.keysym.sym);  
      }
    }
    SDL_Delay(1);
  }


  TTF_Quit();

  Mix_CloseAudio();

  SDL_FreeSurface(sprShip);
  SDL_FreeSurface(sprEnemy_00);
  SDL_FreeSurface(sprEnemy_01);
  SDL_FreeSurface(sprBackground);
  SDL_FreeSurface(sprBullet);

  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
