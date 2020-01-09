//2019 Levi D. Smith - levidsmith.com
#include "globals.h"
#include <stdio.h>
#include <math.h>

#include "linked_list.h"
#include "bullet.h"
#include "ship.h"
#include "enemy.h"

extern SDL_Renderer *renderer;
extern SDL_Texture *imgBullet[6];
extern SDL_Texture *imgBulletEnemy;
extern struct Ship *ship;
extern struct Node *listEnemy;



void init_bullet(struct Bullet *bullet, int init_x, int init_y, int init_level) {
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
  
  bullet->x -= bullet->width / 2;
  bullet->iDamage = 1;
  bullet->iLevel = init_level;
  
  bullet->fSeekRadius = 0;
  bullet->seekEnemy = NULL;
  
  bullet->iDamageMultiplier = 1;
  if (ship->fAttackPowerupDelay > 0) {
	  //bullet->iDamage *= 2;
	  bullet->iDamageMultiplier = 2;
  }

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
		  
		  if (bullet->fLifetime > 10) {
			  bullet->isAlive = FALSE;
		  }

      }
	  
  }
  
  if (bullet->fWaveAmplitude != 0) {
    bullet->x = bullet->orig_x + (bullet->fWaveAmplitude * UNIT_SIZE * sin(bullet->fLifetime * PI * 2));
  }
    
    if (bullet->isSpinShot) {
        bullet->x = ship->x + (ship->width / 2) + 2.0 * UNIT_SIZE * cos(bullet->fLifetime * PI);
        bullet->y = ship->y + (ship->width / 2) + 2.0 * UNIT_SIZE * sin(bullet->fLifetime * PI);
        
    }
	
	if (bullet->fSeekRadius > 0) {
		//may not want to check on every update, since it has to loop through all of the enemies
		seekEnemy_bullet(bullet);
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
		
		if (bullet->iDamageMultiplier > 1) {
			if ((int) (bullet->fLifetime * 8) % 2 == 0) {
				SDL_SetTextureColorMod(imgBullet[bullet->iWeaponType], 255, 0, 0);
			} else {
				SDL_SetTextureColorMod(imgBullet[bullet->iWeaponType], 255, 255, 255);
				
			}
			SDL_RenderCopy(renderer, imgBullet[bullet->iWeaponType], NULL, &pos);

			
		} else {
				SDL_SetTextureColorMod(imgBullet[bullet->iWeaponType], 255, 255, 255);
		}
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

void seekEnemy_bullet(struct Bullet *bullet) {
	struct Enemy *closestEnemy = NULL;
	float fClosestDistance = -1;
	struct Node *current;
	float fDistance;
	
	
//	printf("seekEnemy_bullet start, seek radius %f\n", bullet->fSeekRadius * UNIT_SIZE);
	
	if (bullet->seekEnemy == NULL) {
		
		
  current = listEnemy;
  struct Enemy *enemy;
  while(current != NULL) {
    enemy = (struct Enemy *) current->data;
	if (enemy->isAlive && enemy->fIntroDelay <= 0) {
		fDistance = getDistance(bullet->x, bullet->y, enemy->x, enemy->y);
//		printf("fDistance %f\n", fDistance);
		
		if (fDistance < bullet->fSeekRadius * UNIT_SIZE) {
		
			if (closestEnemy == NULL) {
//				printf("found closest enemy (NULL)\n");
				closestEnemy = enemy;
				fClosestDistance = getDistance(bullet->x, bullet->y, enemy->x, enemy->y);
			} else {
				if (fDistance < fClosestDistance) {
//					printf("found closest enemy (distance)\n");

					closestEnemy = enemy;
					fClosestDistance = fDistance;
				
				}
			
			}
		}
		
	}
    current = current->next;
  }
  
 
	}
	
	bullet->seekEnemy = closestEnemy;

	if (bullet->seekEnemy != NULL) {
//					printf("Seek to enemy\n");


//		fDistance	= getDistance(bullet->seekEnemy->x, bullet->seekEnemy->y, bullet->x, bullet->y);
		fDistance	= getDistance(getCenterX_enemy(bullet->seekEnemy), getCenterY_enemy(bullet->seekEnemy), 
						getCenterX_bullet(bullet), getCenterY_bullet(bullet));
		bullet->vel_x = 5 * (getCenterX_enemy(bullet->seekEnemy) - getCenterX_bullet(bullet)) / fDistance;
		bullet->vel_y = 5 * (getCenterY_enemy(bullet->seekEnemy) - getCenterY_bullet(bullet)) / fDistance;
	}
	
	
}
