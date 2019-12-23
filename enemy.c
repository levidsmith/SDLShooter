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
extern SDL_Texture *imgEnemyAlpha_L1_00;
extern SDL_Texture *imgEnemyAlpha_L1_01;
extern SDL_Texture *imgEnemyAlpha_L2_00;
extern SDL_Texture *imgEnemyAlpha_L2_01;
extern SDL_Texture *imgEnemyBravo_00;
extern SDL_Texture *imgEnemyBravo_01;
extern SDL_Texture *imgEnemyCharlie_00;
extern SDL_Texture *imgEnemyCharlie_01;
extern SDL_Texture *imgEnemyDelta_00;
extern SDL_Texture *imgEnemyDelta_01;
extern SDL_Texture *imgEnemyEcho_00;
extern SDL_Texture *imgEnemyEcho_01;
extern SDL_Texture *imgEnemyEcho_02;
extern SDL_Texture *imgEnemyEcho_03;


extern Mix_Chunk *soundEnemyShoot;

extern Mix_Chunk *soundEnemyHit;
extern Mix_Chunk *soundEnemyDead;
extern Mix_Chunk *soundEnemyShield;


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
  enemy->iHealth = 1;
  enemy->iLevel = 1;
  enemy->fDamagedCountdown = 0;
  enemy->target_x = 0;
  enemy->target_y = 0;
  enemy->iMoveToTarget = FALSE;
  
  setShootDelay_enemy(enemy);
  if (enemy->iType == 4) {
	  //setTargetPosition_enemy(enemy, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
	  //setTargetPosition_enemy(enemy, 400.0, 400.0);
//		setTargetPosition_enemy(enemy, (float) (rand() % 400), (float) (rand() % 400));
//	  setTargetPosition_enemy(enemy, 64 + (rand() % (SCREEN_WIDTH - (64 * 3))), 64 + (rand() % (SCREEN_HEIGHT - (64 * 3))));
	  setTargetPosition_enemy(enemy, (1 + (rand() % ((SCREEN_WIDTH / 64) - 2))) * 64, (1 + (rand() % ((SCREEN_HEIGHT / 64) - 2))) * 64);
  }

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
	  case 4:
	    if (enemy->fWaitCountdown > 0) {
			enemy->fWaitCountdown -= DELTA_TIME;
			if (enemy->fWaitCountdown <= 0) {
				enemy->fWaitCountdown = 0;
//				setTargetPosition_enemy(enemy, rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
//				setTargetPosition_enemy(enemy, 600, 200);
//				enemy->target_x = rand() % SCREEN_WIDTH;
//				enemy->target_y = rand() % SCREEN_HEIGHT;
//				enemy->iMoveToTarget = TRUE;
//				enemy->fAwakeCountdown = 1;
//	  setTargetPosition_enemy(enemy, 64 + (rand() % (SCREEN_WIDTH - (64 * 3))), 64 + (rand() % (SCREEN_HEIGHT - (64 * 3))));
	  setTargetPosition_enemy(enemy, (1 + (rand() % ((SCREEN_WIDTH / 64) - 2))) * 64, (1 + (rand() % ((SCREEN_HEIGHT / 64) - 2))) * 64);

			}
		} else if (enemy->iMoveToTarget) {
			
			if (  getDistance(enemy->x, enemy->y, enemy->target_x, enemy->target_y) > sqrt(pow(enemy->vel_x, 2) + pow(enemy->vel_y, 2)) ) {
				enemy->x += enemy->vel_x;
				enemy->y += enemy->vel_y;
			} else {
				enemy->x = enemy->target_x;
				enemy->y = enemy->target_y;
				enemy->vel_x = 0;
				enemy->vel_y = 0;
				enemy->iMoveToTarget = FALSE;
				enemy->fWaitCountdown = 5;
//				enemy->fShootDelay = 0.5;
			}
			
			/*
			if (enemy->x < enemy->target_x) {
				enemy->x += 1 * UNIT_SIZE * DELTA_TIME;
			} else if (enemy->x > enemy->target_x) {
				enemy->x -= 1 * UNIT_SIZE * DELTA_TIME;
			}
			
			if (enemy->y < enemy->target_y) {
				enemy->y += 1 * UNIT_SIZE * DELTA_TIME;
			} else if (enemy->y > enemy->target_y) {
				enemy->y -= 1 * UNIT_SIZE * DELTA_TIME;
			}
			*/

			
/*
		} else if (enemy->fAwakeCountdown > 0) {
			enemy->fAwakeCountdown -= DELTA_TIME;
			if (enemy->fAwakeCountdown <= 0) {
//				enemy->fAwakeCountdown = 0;
				enemy->fSleepCountdown = 2;
				
			}
*/			
			
		}
		
		
		break;


    }
	
	if (enemy->fShootDelay > 0) {
		enemy->fShootDelay -= DELTA_TIME;
		if (enemy->fShootDelay <= 0) {
			shoot_enemy(enemy);
			setShootDelay_enemy(enemy);
			
			
			
		}
	}
	
	if (enemy->fDamagedCountdown > 0) {
		enemy->fDamagedCountdown -= DELTA_TIME;
		
		if (enemy->fDamagedCountdown < 0) {
			enemy->fDamagedCountdown = 0;
			
		}
		
	}
	

    if (enemy->y > SCREEN_HEIGHT) {
      enemy->y -= SCREEN_HEIGHT;
    }
}

void draw_enemy(struct Enemy *enemy) {
    SDL_Rect pos;
	SDL_Texture *img = NULL;
	
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
			  if (enemy->iLevel == 1) {
				//SDL_RenderCopy(renderer, imgEnemyAlpha_L1_00, NULL, &pos);
				img = imgEnemyAlpha_L1_00;
			  } else if (enemy->iLevel == 2) {
				  //SDL_RenderCopy(renderer, imgEnemyAlpha_L2_00, NULL, &pos);
				img = imgEnemyAlpha_L2_00;
			  }
		  } else if (iSpriteIndex == 1) {
			  if (enemy->iLevel == 1) {
//				SDL_RenderCopy(renderer, imgEnemyAlpha_L1_01, NULL, &pos);
				img = imgEnemyAlpha_L1_01;

			  } else if (enemy->iLevel == 2) {
//				  SDL_RenderCopy(renderer, imgEnemyAlpha_L2_01, NULL, &pos);
				img = imgEnemyAlpha_L2_01;

			  }
		  }

          break;
        case 1:
		  
          if (iSpriteIndex == 0) {
//	        SDL_RenderCopy(renderer, imgEnemyBravo_00, NULL, &pos);
				img = imgEnemyBravo_00;

		  } else if (iSpriteIndex == 1) {
//	        SDL_RenderCopy(renderer, imgEnemyBravo_01, NULL, &pos);
				img = imgEnemyBravo_01;
		  }

          break;
        case 2:
		  
          if (iSpriteIndex == 0) {
//	        SDL_RenderCopy(renderer, imgEnemyCharlie_00, NULL, &pos);
				img = imgEnemyCharlie_00;

		  } else if (iSpriteIndex == 1) {
			  	img = imgEnemyCharlie_01;

//	        SDL_RenderCopy(renderer, imgEnemyCharlie_01, NULL, &pos);
		  }

          break;
        case 3:
		  
          if (iSpriteIndex == 0) {
				img = imgEnemyDelta_00;
			  
//	        SDL_RenderCopy(renderer, imgEnemyDelta_00, NULL, &pos);
		  } else if (iSpriteIndex == 1) {
				img = imgEnemyDelta_01;
			  
//	        SDL_RenderCopy(renderer, imgEnemyDelta_01, NULL, &pos);
		  }

          break;
		  
        case 4:
          if (iSpriteIndex == 0) {
			  if (enemy->fWaitCountdown > 0) {
				img = imgEnemyEcho_00;
			  } else {
				img = imgEnemyEcho_02;
			  }
		  } else if (iSpriteIndex == 1) {
			  if (enemy->fWaitCountdown > 0) {
				img = imgEnemyEcho_01;
			  } else {
				img = imgEnemyEcho_03;
			  }
		  }

          break;
		  
		  
      }
	
		if (img != NULL) {
			if (enemy->fDamagedCountdown > 0) {
					SDL_SetTextureColorMod(img, 255, 0, 0);
			} else {
				SDL_SetTextureColorMod(img, 255, 255, 255);

			}
			SDL_RenderCopy(renderer, img, NULL, &pos);
		}

    }

}



void shoot_enemy(struct Enemy *enemy) {
  struct Bullet *bullet;
	
  if (enemy != NULL && (enemy->fShootDelay <= 0) && enemy->isAlive) {
	bullet = malloc(sizeof(struct Bullet));

//	printf("ship at x: %d y: %d\n", ship->x, ship->y);

	init_bullet(bullet, enemy->x + enemy->width / 2, enemy->y);

	bullet->vel_y = 5;
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

void damage_enemy(struct Enemy *enemy, int iDamageAmount) {
    int iTotalDamage = 0;

	if (enemy->iType == 4) {
		if (enemy->fWaitCountdown > 0) {
			iTotalDamage = iDamageAmount; //can only damage this enemy if it's waiting with eye open
		}
	} else {
		iTotalDamage = iDamageAmount;

	}


	if (iTotalDamage > 0) {
		enemy->iHealth -= iTotalDamage;
		if (enemy->iHealth <= 0) {
			enemy->isAlive = FALSE;
			Mix_PlayChannel(-1, soundEnemyDead, 0);
		
		} else {
			Mix_PlayChannel(-1, soundEnemyHit, 0);

		}
		enemy->fDamagedCountdown = 0.2;
	
		
	} else {
		Mix_PlayChannel(-1, soundEnemyShield, 0);
		
	}
  

}

void setTargetPosition_enemy(struct Enemy *enemy, float x, float y) {
	float fSpeed = 1;
	enemy->target_x = x;
	enemy->target_y = y;
	
	

	printf("Enemy at x: %f, y: %f, Target position x: %f, y: %f\n", enemy->x, enemy->y, enemy->target_x, enemy->target_y);
	
	
				
			float fDistanceX = enemy->target_x - enemy->x;
			float fDistanceY = enemy->target_y - enemy->y;
			float fDistance = sqrt(pow(fDistanceX, 2) + pow(fDistanceY, 2));
//			float fAngle = atan(fDistanceY / fDistanceX);
			
//			printf("x: %f, y: %f, target_x: %f, target_y: %f, angle %f, distance: %f\n", enemy->x, enemy->y, enemy->target_x, enemy->target_y,
//			 fAngle, fHyp);
//			 printf("moving x: %f, y: %f\n", cos(fAngle), sin(fAngle));
			if (fDistance != 0) {
			 enemy->vel_x = fSpeed * cos(acos(fDistanceX / fDistance));
			 enemy->vel_y = fSpeed * sin(asin(fDistanceY / fDistance));
			 	enemy->iMoveToTarget = TRUE;

			} else {
				printf("*** Distance was zero");
			}
			
	
	
}

float getDistance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}

