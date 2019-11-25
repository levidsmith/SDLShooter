#include <SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 1280 
#define SCREEN_HEIGHT 720 

#define TRUE 1
#define FALSE 0

//FUNCTION PROTOTYPES
void handleInput(int, int);
void shoot();

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
} ship;


struct Enemy {
  int iHealth;
  int x;
  int y;
  float vel_x;
  float vel_y;
  float fLifetime;
  int isAlive;
} enemy;

struct Bullet {
  int x;
  int y;
  int isAlive;
} bullet;


int iKeepLooping = TRUE;

int iBackgroundOffset;

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
  ship.x = 320;
  ship.y = 720 - 128;
  ship.isAlive = TRUE;

  enemy.x = 320;
  enemy.y = 64;
  enemy.isAlive = TRUE;

  bullet.isAlive = FALSE;
}

void update() {
  //update ship
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

  //update enemy
  enemy.fLifetime += 0.2;
  int x1 = (((int) enemy.fLifetime) % 640);
  if (x1 > 320) {
    x1 =  640 - x1; 
  } 
  enemy.x = x1;
  enemy.y += enemy.vel_y;

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
  shipPosition.x = ship.x;
  shipPosition.y = ship.y;
//  SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00));
  SDL_BlitSurface(sprShip, NULL, screenSurface, &shipPosition);

//Draw the bullet
  if (bullet.isAlive) {
    pos.x = bullet.x;
    pos.y = bullet.y;
    SDL_BlitSurface(sprBullet, NULL, screenSurface, &pos);
  }
 
//Draw the enemy
  pos.x = enemy.x;
  pos.y = enemy.y;
  SDL_BlitSurface(sprEnemy, NULL, screenSurface, &pos);



  SDL_UpdateWindowSurface( window);
}

void shoot() {
  if (!bullet.isAlive) {
    bullet.isAlive = TRUE;
    bullet.x = ship.x;
    bullet.y = ship.y;
  }
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
