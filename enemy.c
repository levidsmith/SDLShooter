//2019 Levi D. Smith - levidsmith.com
#include <math.h>
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_mixer.h>
#include "enemy.h"
#include "bullet.h"
#include "linked_list.h"
#include "globals.h"

extern SDL_Renderer *renderer;
extern SDL_Texture *imgEnemyAlpha_00;
extern SDL_Texture *imgEnemyAlpha_01;
extern SDL_Texture *imgEnemyBravo_00;
extern SDL_Texture *imgEnemyBravo_01;
extern SDL_Texture *imgEnemyCharlie_00;
extern SDL_Texture *imgEnemyCharlie_01;
extern SDL_Texture *imgEnemyDelta_00;
extern SDL_Texture *imgEnemyDelta_01;


extern Mix_Chunk *soundEnemyShoot;


extern struct Node *listBullet;
extern struct Node *add_node(struct Node **head, void *value);
extern int count_list(struct Node *head);
extern void remove_node(struct Node **head, struct Node *node);





void init_enemy(struct Enemy *enemy, int init_x, int init_y, int init_iType) {
  enemy->x = init_x;
  enemy->y = init_y;
  enemy->orig_x = init_x;
  enemy->orig_y = init_y;
  enemy->vel_x = UNIT_SIZE * 3;
  enemy->vel_y = 0;
  enemy->width = 64;
  enemy->height = 64;
  enemy->fChangeMovementCountdown = 1;
  enemy->isAlive = TRUE;
  enemy->iType = init_iType;
  enemy->fLifetime = 0;
  enemy->fShootDelay = 0;
  enemy->hasDrop = FALSE;
  
  setShootDelay_enemy(enemy);

}

void update_enemy(struct Enemy *enemy) {
    enemy->fLifetime += DELTA_TIME;

    switch(enemy->iType) {
      case 0:
        enemy->fChangeMovementCountdown = enemy->fChangeMovementCountdown - (1 * DELTA_TIME);
        if (enemy->fChangeMovementCountdown <= 0) {
          enemy->vel_x *= -1;
          enemy->y += (UNIT_SIZE / 4);
          enemy->fChangeMovementCountdown = 2;
        }
        enemy->x += enemy->vel_x * DELTA_TIME;
        enemy->y += enemy->vel_y;
        break;
      case 1:  
        enemy->vel_x = (1 * UNIT_SIZE);
        enemy->x += (enemy->vel_x * DELTA_TIME);
        if (enemy->x > SCREEN_WIDTH) {
          enemy->x -= SCREEN_WIDTH;
        }
        break;
	  case 2:
	    enemy->x = enemy->orig_x + 250 * sin(enemy->fLifetime * PI);
		enemy->y += 64 * DELTA_TIME;
		break;
	  case 3:
	    enemy->x = enemy->orig_x + (128 * cos(enemy->fLifetime * PI));
		enemy->y = enemy->orig_y + (128 * sin(enemy->fLifetime * PI));
		break;


    }
	
	if (enemy->fShootDelay > 0) {
		enemy->fShootDelay -= DELTA_TIME;
		if (enemy->fShootDelay <= 0) {
			shoot_enemy(enemy);
			setShootDelay_enemy(enemy);
			
			
			
		}
	}
	

    if (enemy->y > SCREEN_HEIGHT) {
      enemy->y -= SCREEN_HEIGHT;
    }
}

void draw_enemy(struct Enemy *enemy) {
    SDL_Rect pos;
	
//	printf("enemy x: %f y %f\n", enemy->x, enemy->y);
	
	int iSpriteIndex = ((int) (enemy->fLifetime * 2)) % 2; //change sprite every 0.5 seconds
	

    if (enemy->isAlive) {
      pos.x = enemy->x;
      pos.y = enemy->y;
	  pos.w = enemy->width;
	  pos.h = enemy->height;

      switch(enemy->iType) {
        case 0:
          if (iSpriteIndex == 0) {
	        SDL_RenderCopy(renderer, imgEnemyAlpha_00, NULL, &pos);
		  } else if (iSpriteIndex == 1) {
	        SDL_RenderCopy(renderer, imgEnemyAlpha_01, NULL, &pos);
		  }

          break;
        case 1:
		  
          if (iSpriteIndex == 0) {
	        SDL_RenderCopy(renderer, imgEnemyBravo_00, NULL, &pos);
		  } else if (iSpriteIndex == 1) {
	        SDL_RenderCopy(renderer, imgEnemyBravo_01, NULL, &pos);
		  }

          break;
        case 2:
		  
          if (iSpriteIndex == 0) {
	        SDL_RenderCopy(renderer, imgEnemyCharlie_00, NULL, &pos);
		  } else if (iSpriteIndex == 1) {
	        SDL_RenderCopy(renderer, imgEnemyCharlie_01, NULL, &pos);
		  }

          break;
        case 3:
		  
          if (iSpriteIndex == 0) {
	        SDL_RenderCopy(renderer, imgEnemyDelta_00, NULL, &pos);
		  } else if (iSpriteIndex == 1) {
	        SDL_RenderCopy(renderer, imgEnemyDelta_01, NULL, &pos);
		  }

          break;
		  
      }
    }

}



void shoot_enemy(struct Enemy *enemy) {
  struct Bullet *bullet;
	
  if (enemy != NULL && (enemy->fShootDelay <= 0) && enemy->isAlive) {
	bullet = malloc(sizeof(struct Bullet));

//	printf("ship at x: %d y: %d\n", ship->x, ship->y);

	init_bullet(bullet, enemy->x + enemy->width / 2, enemy->y);

	bullet->vel_y = -5;
	bullet->iHitsPlayer = TRUE;

//	printf("added bullet at x: %d y: %d\n", bullet->x, bullet->y);
	
	add_node(&listBullet, bullet);
	Mix_PlayChannel(-1, soundEnemyShoot, 0);

//    Mix_PlayChannel(-1, soundShoot, 0);
  }
}

void setShootDelay_enemy(struct Enemy *enemy) {
  switch(enemy->iType) {
	case 0:
		enemy->fShootDelay = 1 + ((rand() % 50)  * 0.1);  //between 1 and 6 seconds
		break;
	case 1:
		enemy->fShootDelay = 5 + ((rand() % 50) * 0.1); //between 5 and 10 seconds
		break;
  }
	
}

