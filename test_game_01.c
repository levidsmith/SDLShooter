#include <SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 1280 
#define SCREEN_HEIGHT 720 

#define TRUE 1
#define FALSE 0

//FUNCTION PROTOTYPES
void handleInput(int, int);

//SDL variables

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* sprShip;


//VARIABLES
SDL_Rect shipPosition;
struct Ship {
  float vel_x;
  float vel_y;
  int x;
  int y;
  int width;
  int height;
} ship;



int iKeepLooping = TRUE;


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
  ship.y = 240;
}

void update() {
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
//  SDL_BlitSurface(sprShip, NULL, screenSurface, NULL);
  shipPosition.x = ship.x;
  shipPosition.y = ship.y;
  SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00));
  SDL_BlitSurface(sprShip, NULL, screenSurface, &shipPosition);
  SDL_UpdateWindowSurface( window);
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

  
  SDL_Surface* sprHello;
  sprHello = SDL_LoadBMP("hello.bmp");
  sprShip = SDL_LoadBMP("ship.bmp");

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


  SDL_FreeSurface(sprHello);
  SDL_FreeSurface(sprShip);

  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
