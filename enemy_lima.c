//2020 Levi D. Smith
#include <stdlib.h>
#include <math.h>

#include "globals.h"
#include "linked_list.h"
#include "enemy.h"
#include "enemy_lima.h"
#include "ship.h"
#include "util.h"

extern SDL_Texture *imgEnemyLima_L1_00;
extern SDL_Texture *imgEnemyLima_L1_01;
extern SDL_Texture *imgEnemyLima_L1_02;
extern SDL_Texture *imgEnemyLima_L1_03;
extern SDL_Texture *imgEnemyLima_L2_00;
extern SDL_Texture *imgEnemyLima_L2_01;
extern SDL_Texture *imgEnemyLima_L2_02;
extern SDL_Texture *imgEnemyLima_L2_03;
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
        enemy->iHealth = 3;
        enemylima->fSpeed = 4;
        enemylima->fMaxChaseTime = 3;
    }

}
void update_enemy_lima(struct Enemy *enemy) {
    struct EnemyLima *enemylima = (struct EnemyLima *) enemy->subtype;
    float enemy_CenterX = getCenterX_enemy(enemy);
    float enemy_CenterY = getCenterY_enemy(enemy);
    float ship_CenterX = getCenterX_ship(ship);
    float ship_CenterY = getCenterY_ship(ship);

    if (enemylima->isAsleep && getDistance(enemy_CenterX, enemy_CenterY, ship_CenterX, ship_CenterY) < enemylima->fWakeUpRadius * UNIT_SIZE) {
        enemylima->isAsleep = FALSE;
        enemy->orig_x = enemy->x;
        enemylima->fWakeDelay = (1 + getRandomInt(0, 4)) * 0.2;

    }

    if (!enemylima->isAsleep) {
        //        enemy->y += enemylima->fSpeed * UNIT_SIZE * DELTA_TIME;
        if (enemylima->fWakeDelay > 0) {
            enemylima->fWakeDelay -= DELTA_TIME;
            float iShakeDistance = 4;
            enemy->x = enemy->orig_x - (iShakeDistance / 2) + (iShakeDistance * (((int)(enemy->fLifetime * 16)) % 2));

            if (enemylima->fWakeDelay <= 0) {
                //Woke update_enemy_lima
                enemylima->fWakeDelay = 0;
            }
        }
        if (ship->isAlive && enemylima->fWakeDelay <= 0) {
            float fDistanceX = enemy_CenterX - ship_CenterX;
            float fDistanceY = enemy_CenterY - ship_CenterY;
            float fDistance = getDistance(ship_CenterX, ship_CenterY, enemy_CenterX, enemy_CenterY);
            float vel_x = -1 * enemylima->fSpeed * cos(acos(fDistanceX / fDistance));
            float vel_y = -1 * enemylima->fSpeed * sin(asin(fDistanceY / fDistance));

            enemy->x += vel_x;
            enemy->y += vel_y;
            enemylima->fChaseTime += DELTA_TIME;
            if (enemylima->fChaseTime >= enemylima->fMaxChaseTime) {
                enemylima->isAsleep = TRUE;
                enemylima->fChaseTime = 0;
            }
        }

    }

}

SDL_Texture *getTexture_enemy_lima(struct Enemy *enemy) {
    SDL_Texture *img = NULL;
    struct EnemyLima *enemylima = (struct EnemyLima *) enemy->subtype;

    img = NULL;
    int iSpriteIndex = ((int)(enemy->fLifetime * 2)) % 2;

    if (enemylima->isAsleep || enemylima->fWakeDelay > 0) {
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
    } else {
        iSpriteIndex = ((int)(enemy->fLifetime * 8)) % 2;
        if (iSpriteIndex == 0) {
            if (enemy->iLevel == 1) {
                img = imgEnemyLima_L1_02;
            } else if (enemy->iLevel == 2) {
                img = imgEnemyLima_L2_02;
            }
        } else if (iSpriteIndex == 1) {
            if (enemy->iLevel == 1) {
                img = imgEnemyLima_L1_03;
            } else if (enemy->iLevel == 2) {
                img = imgEnemyLima_L2_03;

            }
        }

    }

    return img;
}

void damage_enemy_lima(struct Enemy *enemy, int iDamageAmount) {
    struct EnemyLima *enemylima = (struct EnemyLima *) enemy->subtype;

    if (!enemylima->isAsleep && enemylima->fWakeDelay <= 0) {
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
