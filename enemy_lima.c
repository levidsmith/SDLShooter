//2020 Levi D. Smith
#include <stdlib.h>
#include <math.h>

#include "globals.h"
#include "enemy.h"
#include "enemy_lima.h"
#include "ship.h"

extern SDL_Texture *imgEnemyLima_L1_00;
extern SDL_Texture *imgEnemyLima_L1_01;
extern SDL_Texture *imgEnemyLima_L1_02;
extern SDL_Texture *imgEnemyLima_L1_03;
extern SDL_Texture *imgEnemyLima_L2_00;
extern SDL_Texture *imgEnemyLima_L2_01;
extern SDL_Texture *imgEnemyLima_L1_02;
extern SDL_Texture *imgEnemyLima_L1_03;
extern SDL_Renderer *renderer;

extern Mix_Chunk *soundEnemyHit;
extern Mix_Chunk *soundEnemyShield;

extern struct Ship *ship;

void init_enemy_lima(struct Enemy *enemy) {
    struct EnemyLima *enemylima = malloc(sizeof(struct EnemyLima));
    enemylima->isAsleep = TRUE;
    enemylima->fWakeUpRadius = 2;
    enemylima->fChaseTime = 0;
    enemy->subtype = enemylima;
    
    if (enemy->iLevel == 1) {
        enemy->iHealth = 1;
        enemylima->fSpeed = 2;
        enemylima->fMaxChaseTime = 2;
    } else if (enemy->iLevel == 2) {
        enemy->iHealth = 2;
        enemylima->fSpeed = 5;
        enemylima->fMaxChaseTime = 3;
    }
    
}


void update_enemy_lima(struct Enemy *enemy) {
    struct EnemyLima *enemylima = (struct EnemyLima *) enemy->subtype;
    if (getDistance(enemy->x + (enemy->width / 2), enemy->y + (enemy->height / 2), ship->x + (ship->width / 2), ship->y + (ship->height / 2)) < enemylima->fWakeUpRadius * UNIT_SIZE) {
        enemylima->isAsleep = FALSE;
    }
    
    if (!enemylima->isAsleep) {
        enemy->y += enemylima->fSpeed * UNIT_SIZE * DELTA_TIME;
        
        enemy->vel_x = 
        
        
        enemylima->fChaseTime += DELTA_TIME;
        if (enemylima->fChaseTime >= enemylima->fMaxChaseTime) {
            enemylima->isAsleep = TRUE;
            enemylima->fChaseTime = 0;
        }

    }
    
}

SDL_Texture *getTexture_enemy_lima(struct Enemy *enemy) {
      SDL_Texture *img = NULL;
      
          img = NULL;
        
 
        
            
            int iSpriteIndex = ((int) (enemy->fLifetime * 2)) % 2; //change sprite every 0.5 seconds
            
            
            if (iSpriteIndex == 0) {
                if (enemy->iLevel == 1) {
                    img = imgEnemyLima_L1_00;
                } else if (enemy->iLevel == 2) {
                    img = imgEnemyLima_L2_00;
                }
            } else if (iSpriteIndex == 1) {
                if (enemy->iLevel == 1) {
                    img = imgEnemyLima_L1_01;
                } else if (enemy->iLevel == 2) {
                    img = imgEnemyLima_L2_01;

                }
            }

      
      
      return img;

    
}

void damage_enemy_lima(struct Enemy *enemy, int iDamageAmount) {
    struct EnemyLima *enemylima = (struct EnemyLima *) enemy->subtype;

    if (!enemylima->isAsleep) {
        enemy->iHealth -= iDamageAmount;
    
        if (enemy->iHealth <= 0) {
            kill_enemy(enemy);
        } else {
            enemy->fDamagedCountdown = 0.2;
            Mix_PlayChannel(-1, soundEnemyHit, 0);

        }
    } else {
        Mix_PlayChannel(-1, soundEnemyShield, 0);

    }
}


void destroy_enemy_lima(struct Enemy *enemy) {
    
}
