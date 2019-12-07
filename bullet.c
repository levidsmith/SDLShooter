//2019 Levi D. Smith - levidsmith.com
#include <SDL.h>
#include "bullet.h"
#include "globals.h"

extern SDL_Renderer *renderer;
extern SDL_Texture *imgBullet;
extern SDL_Texture *imgBulletEnemy;

void init_bullet(struct Bullet *bullet, int init_x, int init_y) {
  bullet->x = (float) init_x;
  bullet->y = (float) init_y;
  bullet->vel_y = 0;
  bullet->isAlive = TRUE;
  bullet->width = 16;
  bullet->height = 16;
  bullet->iHitsPlayer = FALSE;
  bullet->iHitsEnemy = FALSE;

}

void update_bullet(struct Bullet *bullet) {
  if (bullet->isAlive) {
    bullet->y -= (bullet->vel_y * UNIT_SIZE * DELTA_TIME);
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
	
	if (bullet->iHitsEnemy) {
		SDL_RenderCopy(renderer, imgBullet, NULL, &pos);
	} else if (bullet->iHitsPlayer) {
		SDL_RenderCopy(renderer, imgBulletEnemy, NULL, &pos);
	} else {
		SDL_RenderCopy(renderer, imgBullet, NULL, &pos);
		
	}
		
  }

	
}