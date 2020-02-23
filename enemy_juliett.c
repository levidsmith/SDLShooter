// 2020 Levi D. Smith
#include <stdlib.h>
#include <math.h>

#include "globals.h"
#include "enemy.h"
#include "enemy_juliett.h"

extern SDL_Texture *imgEnemyJuliett_L1_00;
extern SDL_Texture *imgEnemyJuliett_L1_01;
extern SDL_Texture *imgEnemyJuliett_L2_00;
extern SDL_Texture *imgEnemyJuliett_L2_01;
extern SDL_Renderer *renderer;

extern Mix_Chunk *soundEnemyHit;
extern Mix_Chunk *soundEnemyDead;



void init_enemy_juliett(struct Enemy *enemy) {
	enemy->width = 64;
    enemy->height = 64;
	
	switch(enemy->iLevel) {
		case 1:
			enemy->iHealth = 1;
			break;
		case 2:
			enemy->iHealth = 3;
			break;
		default:
			enemy->iHealth = 2;
		
	}
	
//	enemy->vel_x = 4;
	enemy->orig_x = enemy->x;
	enemy->orig_y = enemy->y;
	
	struct EnemyJuliett *enemyjuliett = malloc(sizeof(struct EnemyJuliett));
//	enemyjuliett->fWaitDelay = 0;
//	enemyjuliett->fMoveXUnits = 0;
//	enemyjuliett->fTargetXUnits = 4;
	
	
	
	enemy->subtype = enemyjuliett;

	set_target_enemy_juliett(enemy);

}

void update_enemy_juliett(struct Enemy *enemy) {
	struct EnemyJuliett *enemyjuliett = (struct EnemyJuliett *) enemy->subtype;
	float fSpeed = 4;
	int iDir;

	switch(enemy->iLevel) {
		case 1:
			fSpeed = 4;
			break;
		case 2:
			fSpeed = 8;
			break;
		
	}


	
	if (enemyjuliett->fWaitDelay <= 0) {
		if (enemyjuliett->fMoveXUnits < enemyjuliett->fTargetXUnits) {
			iDir = 1;
		} else {
			iDir = -1;
		}
		
//		if (enemy->x + (enemy->vel_x * UNIT_SIZE * DELTA_TIME) > enemyjuliett->fTargetX) {
//		if (enemyjuliett->fMoveXUnits >= enemyjuliett->fTargetXUnits) {
//		if (abs(enemyjuliett->fTargetXUnits - enemyjuliett->fMoveXUnits) < DELTA_TIME * fSpeed) {
		float fAmountToMove = DELTA_TIME * iDir * fSpeed;
	    if ( (fAmountToMove > 0 && fAmountToMove > enemyjuliett->fTargetXUnits - enemyjuliett->fMoveXUnits) ||
		     (fAmountToMove < 0 && fAmountToMove < enemyjuliett->fTargetXUnits - enemyjuliett->fMoveXUnits)
			) {
			enemyjuliett->fMoveXUnits = enemyjuliett->fTargetXUnits;
//			enemyjuliett->fWaitDelay = 2;

			switch(enemy->iLevel) {
				case 1:
					enemyjuliett->fWaitDelay = (1 + rand() % 3) * 1;
					break;
				case 2:
					enemyjuliett->fWaitDelay = (1 + rand() % 5) * 0.2;
					break;
		
			}



		} else {
			enemyjuliett->fMoveXUnits += fAmountToMove;
		}
		
		enemy->x = enemy->orig_x + enemyjuliett->fMoveXUnits * UNIT_SIZE;
		enemy->y = enemy->orig_y + ((pow(fabs(enemyjuliett->fMoveXUnits) - 2, 2) - 4) * UNIT_SIZE);

		
	} else {
		enemyjuliett->fWaitDelay -= DELTA_TIME;
		if (enemyjuliett->fWaitDelay <= 0) {
			set_target_enemy_juliett(enemy);
			
		}
	}
	
}

void set_target_enemy_juliett(struct Enemy *enemy) {
	struct EnemyJuliett *enemyjuliett = (struct EnemyJuliett *) enemy->subtype;
	int iDir;
	
			enemyjuliett->fWaitDelay = 0;
			enemy->orig_x = enemy->x;
			enemyjuliett->fMoveXUnits = 0;
			enemyjuliett->fTargetXUnits = 4;
			
			iDir = rand() % 2;
			if (iDir == 1) {
				enemyjuliett->fTargetXUnits *= -1;
			}
			
			if (enemy->orig_x + enemy->width + (enemyjuliett->fTargetXUnits * UNIT_SIZE) > SCREEN_WIDTH) {
				enemyjuliett->fTargetXUnits = -4;
			} else if (enemy->orig_x + (enemyjuliett->fTargetXUnits * UNIT_SIZE) < 0) {
				enemyjuliett->fTargetXUnits = 4;
			}

	
}

SDL_Texture *getTexture_enemy_juliett(struct Enemy *enemy) {
  SDL_Texture *img = NULL;
  
      img = imgEnemyJuliett_L1_00;
    
        if (enemy->fDamagedCountdown > 0) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
        }
    
        
        int iSpriteIndex = ((int) (enemy->fLifetime * 2)) % 2; //change sprite every 0.5 seconds
        
        
        if (iSpriteIndex == 0) {
            if (enemy->iLevel == 1) {
                img = imgEnemyJuliett_L1_00;
            } else if (enemy->iLevel == 2) {
                img = imgEnemyJuliett_L2_00;
            }
        } else if (iSpriteIndex == 1) {
            if (enemy->iLevel == 1) {
                img = imgEnemyJuliett_L1_01;
            } else if (enemy->iLevel == 2) {
                img = imgEnemyJuliett_L2_01;

            }
        }

  
  
  return img;
}

void damage_enemy_juliett(struct Enemy *enemy, int iDamageAmount) {
	enemy->iHealth -= iDamageAmount;
	
    if (enemy->iHealth <= 0) {
		kill_enemy(enemy);
	} else {

	
		enemy->fDamagedCountdown = 0.2;
		Mix_PlayChannel(-1, soundEnemyHit, 0);

	}

	
}
