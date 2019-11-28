//2019 Levi D. Smith - levidsmith.com
#include <SDL.h>
#include "ship.h"
#include "globals.h"

//extern SDL_Surface *screenSurface;
//extern SDL_Surface *sprShip;

extern SDL_Renderer *renderer;
extern SDL_Texture *imgShip;


void init_ship(struct Ship *ship) {
  ship->width = 64;
  ship->height = 64;
  ship->vel_x = 0;
  ship->vel_y = 0;
  ship->x = (SCREEN_WIDTH - ship->width) / 2;
  ship->y = 720 - 128;
  ship->isAlive = TRUE;

}


void update_ship(struct Ship *ship) {
	  //update ship
  if (ship->isAlive) {
    ship->x += ship->vel_x * DELTA_TIME;
    ship->y += ship->vel_y * DELTA_TIME;

    if (ship->x < 0) {
      ship->x = 0;
    }

    if (ship->y < SCREEN_HEIGHT - (3 * ship->height)) {
      ship->y = SCREEN_HEIGHT - (3 * ship->height);
    }

    if (ship->x > SCREEN_WIDTH - (ship->width)) {
      ship->x = SCREEN_WIDTH - (ship->width);
    }

    if (ship->y > SCREEN_HEIGHT - ship->height) {
      ship->y = SCREEN_HEIGHT - ship->height;
    }
  }

	
}

void draw_ship(struct Ship *ship) {
	SDL_Rect pos;
  //Draw the ship
  if (ship->isAlive) {
	pos.x = ship->x;
	pos.y = ship->y;
//    SDL_BlitSurface(sprShip, NULL, screenSurface, &pos);
	pos.w = ship->width;
	pos.h = ship->height;
    SDL_RenderCopy(renderer, imgShip, NULL, &pos);
  }

	
}