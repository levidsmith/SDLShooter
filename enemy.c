//2019 Levi D. Smith - levidsmith.com
#include "globals.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "screen_game.h"
#include "enemy.h"
#include "bullet.h"
#include "powerup.h"
#include "explosion.h"
#include "linked_list.h"
#include "ship.h"
#include "stats.h"


extern SDL_Renderer *renderer;
extern SDL_Texture *imgEnemyAlpha_L1_00;
extern SDL_Texture *imgEnemyAlpha_L1_01;
extern SDL_Texture *imgEnemyAlpha_L2_00;
extern SDL_Texture *imgEnemyAlpha_L2_01;
extern SDL_Texture *imgEnemyBravo_L1_00;
extern SDL_Texture *imgEnemyBravo_L1_01;
extern SDL_Texture *imgEnemyBravo_L2_00;
extern SDL_Texture *imgEnemyBravo_L2_01;
extern SDL_Texture *imgEnemyCharlie_L1_00;
extern SDL_Texture *imgEnemyCharlie_L1_01;
extern SDL_Texture *imgEnemyCharlie_L2_00;
extern SDL_Texture *imgEnemyCharlie_L2_01;
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
extern struct Node *listPowerup;
extern struct Node *listExplosion;
extern struct Node *add_node(struct Node **head, void *value);
extern int count_list(struct Node *head);
extern void remove_node(struct Node **head, struct Node *node);
//extern int iScore;
extern struct Stats *stats;
extern struct Ship *ship;
//extern void updateScoreText();




void init_enemy(struct Enemy *enemy, int init_x, int init_y, int init_iType, int init_iLevel) {
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
  enemy->iLevel = init_iLevel;
  enemy->fDamagedCountdown = 0;
  enemy->target_x = 0;
  enemy->target_y = 0;
  enemy->iMoveToTarget = FALSE;
  enemy->iPoints = 50;
  
  setShootDelay_enemy(enemy);
    
    switch(enemy->iType) {
        case 2:
            //Charlie
            if (enemy->iLevel == 1) {
                enemy->vel_y = (UNIT_SIZE / 2);
            } else if (enemy->iLevel == 2) {
                enemy->fChangeMovementCountdown = getRandomInt(1, 3);
                enemy->vel_y = UNIT_SIZE;
                //enemy->fWaitCountdown = 2;
            
            }
            break;
        case 4:
            //Echo
            setTargetPosition_enemy(enemy, (1 + (rand() % ((SCREEN_WIDTH / 64) - 2))) * 64, (1 + (rand() % ((SCREEN_HEIGHT / 64) - 2))) * 64);
            break;
            
    }

}

void update_enemy(struct Enemy *enemy) {
    enemy->fLifetime += DELTA_TIME;

    switch(enemy->iType) {
      case 0:
            //Alpha
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
            //Bravo
		if (enemy->iLevel == 1) {
			enemy->vel_x = (1 * UNIT_SIZE);
		} else if (enemy->iLevel == 2) {
			enemy->vel_x = (2.5 * UNIT_SIZE);
		}
		
        enemy->x += (enemy->vel_x * DELTA_TIME);
        if (enemy->x > SCREEN_WIDTH) {
          enemy->x -= SCREEN_WIDTH + enemy->width;
        }
        break;


      case 2:
            //Charlie
	    if (enemy->iLevel == 1) {
			enemy->x = enemy->orig_x + 100 * sin(enemy->fLifetime * PI);
			enemy->y += enemy->vel_y * DELTA_TIME;
			
		} else if (enemy->iLevel == 2) {
			enemy->x = enemy->orig_x + 250 * sin(enemy->fLifetime * PI);
            
            if (enemy->fChangeMovementCountdown > 0) {
                enemy->fChangeMovementCountdown -= DELTA_TIME;

                if (enemy->fChangeMovementCountdown <= 0) {
                    enemy->fChangeMovementCountdown = 0;
                    enemy->fWaitCountdown = getRandomInt(2, 4);
                }
                
                enemy->y += enemy->vel_y * DELTA_TIME;
                
            } else if (enemy->fWaitCountdown > 0) {
                enemy->fWaitCountdown -= DELTA_TIME;
                if (enemy->fWaitCountdown <= 0) {
                    enemy->fWaitCountdown = 0;
                    enemy->fChangeMovementCountdown = getRandomInt(2, 8);
                    
                    int iVel = rand() % 2;
                    if (iVel == 0) {
                        enemy->vel_y *= -1;
                    }
                    
                }
            }
		}
		break;

    case 3:
            //Delta
	    enemy->x = enemy->orig_x + (128 * cos(enemy->fLifetime * PI));
		enemy->y = enemy->orig_y + (128 * sin(enemy->fLifetime * PI));
		break;

      case 4:
            //Echo
	    if (enemy->fWaitCountdown > 0) {
			enemy->fWaitCountdown -= DELTA_TIME;
			if (enemy->fWaitCountdown <= 0) {
				enemy->fWaitCountdown = 0;
				setTargetPosition_enemy(enemy, (1 + (rand() % ((SCREEN_WIDTH / 64) - 2))) * 64, (1 + (rand() % ((SCREEN_HEIGHT / 64) - 2))) * 64);
				//setTargetPosition_enemy(enemy, 5 * 64, 5 * 64);

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
			}
			
			
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
	
    //wrap enemy around top/bottom of screen
    if (enemy->y > SCREEN_HEIGHT) {
      enemy->y -= SCREEN_HEIGHT + enemy->height;
    } else if (enemy->y + enemy->height < 0) {
        enemy->y += SCREEN_HEIGHT;
        
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
			  if (enemy->iLevel == 1) {
				img = imgEnemyBravo_L1_00;
			  } else if (enemy->iLevel == 2) {
				img = imgEnemyBravo_L2_00;
			  }
		  } else if (iSpriteIndex == 1) {
			  if (enemy->iLevel == 1) {
				img = imgEnemyBravo_L1_01;

			  } else if (enemy->iLevel == 2) {
				img = imgEnemyBravo_L2_01;

			  }
		  }

          break;
        case 2:
		  
          if (iSpriteIndex == 0) {
			  if (enemy->iLevel == 1) {
				img = imgEnemyCharlie_L1_00;
			  } else if (enemy->iLevel == 2) {
				img = imgEnemyCharlie_L2_00;
			  }
		  } else if (iSpriteIndex == 1) {
			  if (enemy->iLevel == 1) {
				img = imgEnemyCharlie_L1_01;

			  } else if (enemy->iLevel == 2) {
				img = imgEnemyCharlie_L2_01;

			  }
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
	float fDistance;
	
  if (enemy != NULL && (enemy->fShootDelay <= 0) && enemy->isAlive) {

	  switch(enemy->iType) {
	     case 0:
             case 1:
	bullet = malloc(sizeof(struct Bullet));

	init_bullet(bullet, enemy->x + enemy->width / 2, enemy->y);

	bullet->vel_y = 5;
	bullet->iHitsPlayer = TRUE;
	
	add_node(&listBullet, bullet);
	Mix_PlayChannel(-1, soundEnemyShoot, 0);
 	 	    break;

	     case 3:
	bullet = malloc(sizeof(struct Bullet));

	init_bullet(bullet, enemy->x + enemy->width / 2, enemy->y);

        fDistance	= getDistance(ship->x, ship->y, bullet->x, bullet->y);
	bullet->vel_x = 5 * (getCenterX_ship(ship) - getCenterX_bullet(bullet)) / fDistance;
	bullet->vel_y = 5 * (getCenterY_ship(ship) - getCenterY_bullet(bullet)) / fDistance;
	bullet->iHitsPlayer = TRUE;
	
	add_node(&listBullet, bullet);
	Mix_PlayChannel(-1, soundEnemyShoot, 0);

		    break;
		    
	  }

  }
}

void setShootDelay_enemy(struct Enemy *enemy) {
  switch(enemy->iType) {
	case 0:
		enemy->fShootDelay = 1 + ((rand() % 50)  * 0.1);  //between 1 and 6 seconds
		break;
	case 1:
		if (enemy->iLevel == 1) {
			enemy->fShootDelay = 5 + ((rand() % 50) * 0.1); //between 5 and 10 seconds
		} else if (enemy->iLevel == 2) {
			enemy->fShootDelay = 1; //between 2 and 4 seconds
			
		}
		break;
	case 3:
		enemy->fShootDelay = 2; //2 seconds
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
			destroy_enemy(enemy);
//			Mix_PlayChannel(-1, soundEnemyDead, 0);
		
		} else {
			Mix_PlayChannel(-1, soundEnemyHit, 0);

		}
		enemy->fDamagedCountdown = 0.2;
	
		
	} else {
		Mix_PlayChannel(-1, soundEnemyShield, 0);
		
	}
  

}

void destroy_enemy(struct Enemy *enemy) {
				enemy->isAlive = FALSE;
				//iScore += enemy->iPoints;
				stats->iScore += enemy->iPoints;
				
//					    updateScoreText();
				Mix_PlayChannel(-1, soundEnemyDead, 0);
				
			
				if (enemy->hasDrop) {
					int iType;
					iType = rand() % 4;
					
					struct Powerup *powerup = malloc(sizeof(struct Powerup));
					init_powerup(powerup, enemy->x, enemy->y, iType);
					add_node(&listPowerup, powerup);
				}
				
				struct Explosion *explosion = malloc(sizeof(struct Explosion));
				init_explosion(explosion, enemy->x + (enemy->width / 2), enemy->y + (enemy->height / 2), enemy->width / 2);
				explosion->c.r = 255;
				explosion->c.g = 128;
				explosion->c.b = 128;
				
				add_node(&listExplosion, explosion);
				
			  updateDisplayText();


	
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
				//should okay, enemy will just have another wake cycle at same location
				enemy->iMoveToTarget = TRUE;
			}
			
	
	
}

float getDistance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}

