//2019 Levi D. Smith - levidsmith.com
#include <SDL.h>
#include "bullet.h"
#include "globals.h"

extern SDL_Renderer *renderer;
extern SDL_Texture *imgBullet;

void init_bullet(struct Bullet *bullet, int init_x, int init_y) {
  bullet->x = (float) init_x;
  bullet->y = (float) init_y;
  bullet->isAlive = TRUE;
  bullet->width = 16;
  bullet->height = 16;

}

void update_bullet(struct Bullet *bullet) {
  if (bullet->isAlive) {
    bullet->y -= (5 * UNIT_SIZE * DELTA_TIME);
    if (bullet->y < 0) {
      bullet->isAlive = FALSE;
    }
  }

}

void draw_bullet(struct Bullet *bullet) {
  SDL_Rect pos;
	
  if (bullet->isAlive) {
    pos.x = bullet->x;
    pos.y = bullet->y;
	pos.w = bullet->width;
	pos.h = bullet->height;
    SDL_RenderCopy(renderer, imgBullet, NULL, &pos);
  }

	
}