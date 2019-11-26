#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1280 
#define SCREEN_HEIGHT 720 

#define TRUE 1
#define FALSE 0

//SDL variables

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* sprShip;
SDL_Surface* sprEnemy;
SDL_Surface* sprBackground;
SDL_Surface* sprBullet;


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
struct Enemy *enemy;
struct Enemy enemyList[5];

struct Bullet {
  int x;
  int y;
  int width;
  int height;
  int isAlive;
};
struct Bullet bullet;


int iKeepLooping = TRUE;

int iBackgroundOffset;

//FUNCTION PROTOTYPES
void handleInput(int, int);
void shoot();
void checkCollisions();
void addEnemy(struct Enemy *, int, int);


//FUNCTIONS

void start() {
/*
  shipPosition.x = 320;
  shipPosition.y = 240;
  shipPosition.w = 64;
  shipPosition.h = 64;
*/

  ship.width = 64;
  ship.height = 64;
  ship.vel_x = 0;
  ship.vel_y = 0;
  ship.x = (SCREEN_WIDTH - ship.width) / 2;
  ship.y = 720 - 128;
  ship.isAlive = TRUE;

  enemy = malloc(sizeof(struct Enemy));
  enemy->x = 320;
  enemy->y = 64;
  enemy->width = 64;
  enemy->height = 64;
  enemy->isAlive = TRUE;

  struct Enemy e1;
  addEnemy(&e1, (SCREEN_WIDTH - 64) / 2, 64 + (80 * 0));

  struct Enemy e2;
  addEnemy(&e2, (SCREEN_WIDTH - 64) / 2, 64 + (80 * 1));

  struct Enemy e3;
  addEnemy(&e3, (SCREEN_WIDTH - 64) / 2, 64 + (80 * 2));

  struct Enemy e4;
  addEnemy(&e4, (SCREEN_WIDTH - 64) / 2, 64 + (80 * 3));

  struct Enemy e5;
  addEnemy(&e5, (SCREEN_WIDTH - 64) / 2, 64 + (80 * 4));

  enemyList[0] = e1; 
  enemyList[1] = e2; 
  enemyList[2] = e3; 
  enemyList[3] = e4; 
  enemyList[4] = e5; 


/*
  struct Enemy *enemyTest = &enemyList[2];
  enemyTest->x = 0;
*/
  

  bullet.isAlive = FALSE;
  bullet.width = 16;
  bullet.height = 16;
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

  //update enemy
  enemy->fLifetime += 0.2;
  int x1 = (((int) enemy->fLifetime) % 640);
  if (x1 > 320) {
    x1 =  640 - x1; 
  } 
  enemy->x = x1;
  enemy->y += enemy->vel_y;

  //Update the enemies
  for (i = 0; i < 5; i++) {
    enemyList[i].fLifetime += 0.2;
/*
    x1 = (((int) enemyList[i].fLifetime) % 640);
    if (x1 > 320) {
      x1 =  640 - x1; 
    } 
*/

    enemyList[i].fChangeMovementCountdown = enemyList[i].fChangeMovementCountdown - 1;
    if (enemyList[i].fChangeMovementCountdown <= 0) {
      enemyList[i].vel_x *= -1; 
      enemyList[i].fChangeMovementCountdown = 60 * 10; 
    } 
    enemyList[i].x += enemyList[i].vel_x;
    enemyList[i].y += enemyList[i].vel_y;

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
/*
  if ( (enemy->isAlive) && 
       (bullet.x >= enemy->x && bullet.x < enemy->x + enemy->width) &&
       (bullet.y >= enemy->y && bullet.y < enemy->y + enemy->height) ) {
    bullet.isAlive = FALSE;
    enemy->isAlive = FALSE;
  } 

  if ( (enemy->isAlive) &&  (ship.isAlive) &&
       (ship.x >= enemy->x && ship.x < enemy->x + enemy->width) &&
       (ship.y >= enemy->y && ship.y < enemy->y + enemy->height) ) {
    ship.isAlive = FALSE;
  } 
*/

  int i;

  //Update the enemies
  for (i = 0; i < 5; i++) {
    if ( (enemyList[i].isAlive) && (bullet.isAlive) && 
         (bullet.x >= enemyList[i].x && bullet.x < enemyList[i].x + enemyList[i].width) &&
         (bullet.y >= enemyList[i].y && bullet.y < enemyList[i].y + enemyList[i].height) ) {
      bullet.isAlive = FALSE;
      enemyList[i].isAlive = FALSE;
    } 

    if ( (enemyList[i].isAlive) &&  (ship.isAlive) &&
         (ship.x >= enemyList[i].x && ship.x < enemyList[i].x + enemyList[i].width) &&
         (ship.y >= enemyList[i].y && ship.y < enemyList[i].y + enemyList[i].height) ) {
      ship.isAlive = FALSE;
    } 



  }



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
 
//Draw the enemy
/*
  if (enemy->isAlive) {
    pos.x = enemy->x;
    pos.y = enemy->y;
    SDL_BlitSurface(sprEnemy, NULL, screenSurface, &pos);
  }
*/

//Draw the enemies
  for (i = 0; i < 5; i++) {
    if (enemyList[i].isAlive) {
      pos.x = enemyList[i].x;
      pos.y = enemyList[i].y;
      SDL_BlitSurface(sprEnemy, NULL, screenSurface, &pos);
    }
  }


  SDL_UpdateWindowSurface( window);
}

void shoot() {
  if (ship.isAlive && !bullet.isAlive) {
    bullet.isAlive = TRUE;
    bullet.x = ship.x + ((ship.width - bullet.width) / 2);
    bullet.y = ship.y;
  }
}

void addEnemy(struct Enemy *e, int init_x, int init_y) {
  e->x = init_x;
  e->y = init_y;
  e->vel_x = 0.2;
  e->vel_y = 0;
  e->width = 64;
  e->height = 64;
  e->fChangeMovementCountdown = 60 * 10;
  e->isAlive = TRUE;
}

int main(int argc, char* args[]) {


  if (SDL_Init( SDL_INIT_VIDEO) < 0) {
    printf("Error: %s\n", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Error: %s\n", SDL_GetError());
    return 1;
  }
    
  screenSurface = SDL_GetWindowSurface(window);

  
//  SDL_Surface* sprHello;
//  sprHello = SDL_LoadBMP("hello.bmp");
  sprShip = SDL_LoadBMP("ship.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  sprBackground = SDL_LoadBMP("background.bmp");

  sprEnemy = SDL_LoadBMP("enemy.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));

  sprBullet = SDL_LoadBMP("bullet.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));

//  SDL_BlitSurface(sprHello, NULL, screenSurface, NULL);
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


//  SDL_FreeSurface(sprHello);
  SDL_FreeSurface(sprShip);
  SDL_FreeSurface(sprEnemy);
  SDL_FreeSurface(sprBackground);
  SDL_FreeSurface(sprBullet);

  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
