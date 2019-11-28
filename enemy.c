//2019 Levi D. Smith - levidsmith.com
#include <SDL.h>
#include "enemy.h"
#include "globals.h"

extern SDL_Surface *screenSurface;
extern SDL_Surface *sprEnemy_00;
extern SDL_Surface *sprEnemy_01;


void init_enemy(struct Enemy *enemy, int init_x, int init_y) {
  enemy->x = init_x;
  enemy->y = init_y;
  enemy->vel_x = 0.2;
  enemy->vel_y = 0;
  enemy->width = 64;
  enemy->height = 64;
  enemy->fChangeMovementCountdown = 60 * 10;
  enemy->isAlive = TRUE;
  enemy->iType = 0;

}

void update_enemy(struct Enemy *enemy) {
    enemy->fLifetime += 0.2;

    switch(enemy->iType) {
      case 0:
        enemy->fChangeMovementCountdown = enemy->fChangeMovementCountdown - 1;
        if (enemy->fChangeMovementCountdown <= 0) {
          enemy->vel_x *= -1;
          enemy->y += 16;
          enemy->fChangeMovementCountdown = 60 * 20;
        }
        enemy->x += enemy->vel_x;
        enemy->y += enemy->vel_y;
        break;
      case 1:  
        enemy->vel_x = 1;
        enemy->x += enemy->vel_x;
        if (enemy->x > SCREEN_WIDTH) {
          enemy->x -= SCREEN_WIDTH;
        }
        break;
    }
}

void draw_enemy(struct Enemy *enemy) {
    SDL_Rect pos;

    if (enemy->isAlive) {
      pos.x = enemy->x;
      pos.y = enemy->y;

      switch(enemy->iType) {
        case 0:
          SDL_BlitSurface(sprEnemy_00, NULL, screenSurface, &pos);
          break;
        case 1:
          SDL_BlitSurface(sprEnemy_01, NULL, screenSurface, &pos);
          break;
      }
    }

}
