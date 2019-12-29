//2019 Levi D. Smith - levidsmith.com
#include "globals.h"
#include <math.h>

#include "linked_list.h"
#include "bullet.h"
#include "ship.h"

extern SDL_Renderer *renderer;
extern SDL_Texture *imgBullet[6];
extern SDL_Texture *imgBulletEnemy;
extern struct Ship *ship;

void init_bullet(struct Bullet *bullet, int init_x, int init_y) {
  bullet->x = (float) init_x;
  bullet->y = (float) init_y;
  bullet->orig_x = bullet->x;
  bullet->orig_y = bullet->y;
  bullet->vel_x = 0;
  bullet->vel_y = 0;
  bullet->isAlive = TRUE;
  bullet->width = 16;
  bullet->height = 16;
  bullet->iHitsPlayer = FALSE;
  bullet->iHitsEnemy = FALSE;
  bullet->fLifetime = 0;
  bullet->fWaveAmplitude = 0;
  bullet->fBlastRadius = 0;
  bullet->isSpinShot = FALSE;
  bullet->iWeaponType = 0;
  

}

void update_bullet(struct Bullet *bullet) {
	bullet->fLifetime += DELTA_TIME;
	
  if (bullet->isAlive) {
      if (!bullet->isSpinShot) {
          bullet->x += bullet->vel_x * UNIT_SIZE * DELTA_TIME;
          bullet->y += bullet->vel_y * UNIT_SIZE * DELTA_TIME;
          if (bullet->y < 0 || bullet->y > SCREEN_HEIGHT) {
              bullet->isAlive = FALSE;
          }
      }
  }
  
  if (bullet->fWaveAmplitude != 0) {
    bullet->x = bullet->orig_x + (bullet->fWaveAmplitude * UNIT_SIZE * sin(bullet->fLifetime * PI * 2));
//    bullet->x = bullet->orig_x + (bullet->fWaveAmplitude * UNIT_SIZE);
  }
    
    if (bullet->isSpinShot) {
        bullet->x = ship->x + (ship->width / 2) + 2.0 * UNIT_SIZE * cos(bullet->fLifetime * PI);
        bullet->y = ship->y + (ship->width / 2) + 2.0 * UNIT_SIZE * sin(bullet->fLifetime * PI);
        
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
		SDL_RenderCopy(renderer, imgBullet[bullet->iWeaponType], NULL, &pos);
	} else if (bullet->iHitsPlayer) {
		SDL_RenderCopy(renderer, imgBulletEnemy, NULL, &pos);
	} else {
		SDL_RenderCopy(renderer, imgBullet[bullet->iWeaponType], NULL, &pos);
		
	}
		
  }

	
}

float getCenterX_bullet(struct Bullet *bullet) {
	return bullet->x + (bullet->width / 2);
}

float getCenterY_bullet(struct Bullet *bullet) {
	return bullet->y + (bullet->height / 2);
}
