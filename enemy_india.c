//
//  enemy_india.c
//  sdl-shooter-mac
//
//  Created by Levi D. Smith on 1/24/20.
//  Copyright © 2020 Levi D. Smith. All rights reserved.
//

#include <stdlib.h>
#include <math.h>

#include "globals.h"
#include "enemy.h"
#include "enemy_india.h"


extern SDL_Texture *imgEnemyIndia_L1_00;
extern SDL_Texture *imgEnemyIndia_L1_01;
extern SDL_Texture *imgEnemyIndia_L2_00;
extern SDL_Texture *imgEnemyIndia_L2_01;
extern SDL_Texture *imgEnemyIndia_L3_00;
extern SDL_Texture *imgEnemyIndia_L3_01;

extern SDL_Renderer *renderer;



void init_enemy_india(struct Enemy *enemy) {
    float fAngleRadians;
    switch (enemy->iLevel) {
                case 1:
                    enemy->width = 128;
                    enemy->height = 128;
                    enemy->iHealth = 6;
            fAngleRadians = (rand() % 360) * M_PI / 180;
                    enemy->vel_x = 0.5 * cos(fAngleRadians);
                    enemy->vel_y = 0.5 * sin(fAngleRadians);
                    break;
                case 2:
                    enemy->width = 64;
                    enemy->height = 64;
                    enemy->iHealth = 3;
                    enemy->fIntroDelay = 0;
                    break;
                case 3:
                    enemy->width = 32;
                    enemy->height = 32;
                    enemy->iHealth = 1;
                    enemy->fIntroDelay = 0;
                    break;
    }
    
}

void update_enemy_india(struct Enemy *enemy) {
    enemy->x += enemy->vel_x;
    enemy->y += enemy->vel_y;
    
    if (enemy->x + enemy->width < 0) {
        enemy->x += SCREEN_WIDTH;
    } else if (enemy->x > SCREEN_WIDTH) {
        enemy->x -= SCREEN_WIDTH + enemy->width;
    }
    
}

void damage_enemy_india(struct Enemy *enemy, int iDamageAmount) {
    enemy->iHealth -= iDamageAmount;
    if (enemy->iHealth <= 0) {
        struct Enemy *e1;
        int i;
        int iToSpawn;
        float fNormalizedX;
        float fNormalizedY;
        float fRandomAngleRadians;

        switch(enemy->iLevel) {
            case 1:
                iToSpawn = 4;
//                fRandomAngleRadians = (random() % (360 / iToSpawn)) * M_PI / 180;

                for (i = 0; i < iToSpawn; i++) {
                    e1 = malloc(sizeof(struct Enemy));
//                    fNormalizedX = cos(fRandomAngleRadians + (i * 2 * M_PI / iToSpawn));
//                    fNormalizedY = sin(fRandomAngleRadians + (i * 2 * M_PI / iToSpawn));
                    fNormalizedX = cos(i * 2 * M_PI / iToSpawn);
                    fNormalizedY = sin(i * 2 * M_PI / iToSpawn);

                    init_enemy(e1, enemy->x + (enemy->width / 2) - (e1->width / 2),
                                    enemy->y + (enemy->height / 2) - (e1->height / 2), 8, 2, TRUE);
                    configure_enemy(e1);
                    e1->vel_x = 2 * fNormalizedX;
                    e1->vel_y = 2 * fNormalizedY;
                }

//                e1 = malloc(sizeof(struct Enemy));
//                init_enemy(e1, enemy->x - UNIT_SIZE, enemy->y, 8, 2, TRUE);
//                configure_enemy(e1);


                break;
            case 2:
                iToSpawn = 8;

                for (i = 0; i < iToSpawn; i++) {
                    fNormalizedX = cos(i * 2 * M_PI / iToSpawn);
                    fNormalizedY = sin(i * 2 * M_PI / iToSpawn);
                    e1 = malloc(sizeof(struct Enemy));
                    init_enemy(e1, enemy->x + (enemy->width / 2) - (e1->width / 2),
                                    enemy->y + (enemy->height / 2) - (e1->height / 2), 8, 3, TRUE);
                    configure_enemy(e1);
                    e1->vel_x = 4 * fNormalizedX;
                    e1->vel_y = 4 * fNormalizedY;
                }
                /*
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, enemy->x + UNIT_SIZE, enemy->y, 8, 3, TRUE);
                configure_enemy(e1);

                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, enemy->x - UNIT_SIZE, enemy->y, 8, 3, TRUE);
                configure_enemy(e1);

                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, enemy->x, enemy->y + UNIT_SIZE, 8, 3, TRUE);
                configure_enemy(e1);

                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, enemy->x, enemy->y - UNIT_SIZE, 8, 3, TRUE);
                configure_enemy(e1);
                */

                break;
            case 3:
                break;
        }
        destroy_enemy(enemy);
    
    }
    
        enemy->fDamagedCountdown = 0.2;
    
}

SDL_Texture *getTexture_enemy_india(struct Enemy *enemy) {
    SDL_Texture *img = NULL;
    
    img = imgEnemyIndia_L1_00;
    
        if (enemy->fDamagedCountdown > 0) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
        }
    
        
        int iSpriteIndex = ((int) (enemy->fLifetime * 2)) % 2; //change sprite every 0.5 seconds
        
        
        if (iSpriteIndex == 0) {
            if (enemy->iLevel == 1) {
                img = imgEnemyIndia_L1_00;
            } else if (enemy->iLevel == 2) {
                img = imgEnemyIndia_L2_00;
                } else if (enemy->iLevel == 3) {
                    img = imgEnemyIndia_L3_00;
            }
        } else if (iSpriteIndex == 1) {
            if (enemy->iLevel == 1) {
                img = imgEnemyIndia_L1_01;
            } else if (enemy->iLevel == 2) {
                img = imgEnemyIndia_L2_01;
                } else if (enemy->iLevel == 3) {
                    img = imgEnemyIndia_L3_01;

            }
        }


        
    
    return img;

}
