//2019 Levi D. Smith - levidsmith.com

#include "globals.h"
#include "enemy.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "screen_game.h"
#include "bullet.h"
#include "powerup.h"
#include "explosion.h"
#include "linked_list.h"
#include "ship.h"
#include "stats.h"
#include "util.h"
#include "enemy_golf.h"
#include "enemy_hotel.h"
#include "enemy_india.h"
#include "enemy_juliett.h"
#include "enemy_kilo.h"
#include "enemy_lima.h"
#include "enemy_mike.h"

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
extern SDL_Texture *imgEnemyDelta_L1_00;
extern SDL_Texture *imgEnemyDelta_L1_01;
extern SDL_Texture *imgEnemyEcho_L1_00;
extern SDL_Texture *imgEnemyEcho_L1_01;
extern SDL_Texture *imgEnemyEcho_L1_02;
extern SDL_Texture *imgEnemyEcho_L1_03;
extern SDL_Texture *imgEnemyFoxtrot_L1_00;
extern SDL_Texture *imgEnemyFoxtrot_L1_01;
extern SDL_Texture *imgEnemyFoxtrot_L2_00;
extern SDL_Texture *imgEnemyFoxtrot_L2_01;

extern SDL_Texture *imgEnemyWarp;
extern Mix_Chunk *soundEnemyShoot;

extern Mix_Chunk *soundEnemyHit;
extern Mix_Chunk *soundEnemyDead;
extern Mix_Chunk *soundEnemyShield;

extern struct Node *listEnemy;
extern struct Node *listBullet;
extern struct Node *listPowerup;
extern struct Node *listExplosion;
extern struct Node *add_node(struct Node **head, void *value);
extern int count_list(struct Node *head);
extern void remove_node(struct Node **head, struct Node *node);
extern struct Stats *stats;
extern struct Ship *ship;

void init_enemy(struct Enemy *enemy, int init_x, int init_y, int init_iType, int init_iLevel, int isRoot) {
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
    enemy->fActiveTime = 0;
    enemy->fShootDelay = 0;
    enemy->hasDrop = FALSE;
    enemy->iHealth = 1;
    enemy->iLevel = init_iLevel;
    enemy->fDamagedCountdown = 0;
    enemy->target_x = 0;
    enemy->target_y = 0;
    enemy->iMoveToTarget = FALSE;
    enemy->iPoints = 50;
    enemy->fIntroDelay = 1;
    enemy->fFreezeDelay = 0;
    enemy->fMaxFreezeDelay = 0;
    enemy->fDeathDelay = 0;
    enemy->fMaxDeathDelay = 0.5;
    enemy->isRoot = isRoot;

    setShootDelay_enemy(enemy);

    add_node(&listEnemy, enemy);

    switch (enemy->iType) {
        case id_charlie:
            //Charlie
            if (enemy->iLevel == 1) {
                enemy->vel_y = (UNIT_SIZE / 2);
            } else if (enemy->iLevel == 2) {
                enemy->fChangeMovementCountdown = getRandomInt(1, 3);
                enemy->vel_y = UNIT_SIZE;

            }
            break;

        case id_echo:
            //Echo
            setTargetPosition_enemy(enemy, (1 + (rand() % ((SCREEN_WIDTH / 64) - 2))) * 64, (1 + (rand() % ((SCREEN_HEIGHT / 64) - 2))) * 64);
            break;
        case id_foxtrot:
            enemy->fChangeMovementCountdown = 0;
            break;
        case id_golf:
            if (isRoot) {
                init_enemy_golf(enemy);
            }
            break;
        case id_hotel:
            init_enemy_hotel(enemy);
            break;
        case id_india:
            init_enemy_india(enemy);
            break;
        case id_juliett:
            init_enemy_juliett(enemy);
            break;
        case id_kilo:
            if (isRoot) {
                init_enemy_kilo(enemy);
            }
            break;
        case id_lima:
            init_enemy_lima(enemy);
            break;
        case id_mike:
            if (isRoot) {
                init_enemy_mike(enemy);
            }
            break;

    }

}

void configure_enemy(struct Enemy *enemy) {
    switch (enemy->iType) {
        //Alpha
        case id_alpha:
            switch (enemy->iLevel) {
                case 1:
                    enemy->iPoints = 50;
                    break;
                case 2:
                    enemy->iPoints = 200;
                    enemy->iHealth = 3;
                    enemy->iPoints = 200;
                    break;
            }

            break;
            //Bravo
        case id_bravo:
            switch (enemy->iLevel) {
                case 1:
                    enemy->iPoints = 75;
                    break;
                case 2:
                    enemy->iPoints = 200;
                    enemy->iHealth = 2;
                    break;
            }
            break;

            //Charlie
        case id_charlie:
            switch (enemy->iLevel) {
                case 1:
                    enemy->iPoints = 250;
                    enemy->iHealth = 3;

                    break;
                case 2:
                    enemy->iPoints = 500;
                    enemy->iHealth = 5;

                    break;
            }
            break;

            //Delta
        case id_delta:
            switch (enemy->iLevel) {
                case 1:
                    enemy->iPoints = 500;
                    enemy->iHealth = 5;

                    break;
                case 2:
                    break;
            }
            break;

            //Echo
        case id_echo:
            switch (enemy->iLevel) {
                case 1:
                    enemy->iPoints = 150;
                    enemy->iHealth = 3;

                    break;
                case 2:
                    break;
            }
            break;

            //Foxtrot
        case id_foxtrot:
            switch (enemy->iLevel) {
                case 1:
                    enemy->iPoints = 150;
                    break;
                case 2:
                    enemy->iPoints = 300;
                    enemy->iHealth = 4;
                    break;
            }
            break;
    }
}
void update_enemy(struct Enemy *enemy) {
    if (enemy->fIntroDelay > 0) {
        enemy->fIntroDelay -= DELTA_TIME;
        if (enemy->fIntroDelay <= 0) {
            enemy->fIntroDelay = 0;
        }
    } else if (enemy->fFreezeDelay > 0) {
        enemy->fFreezeDelay -= DELTA_TIME;
        if (enemy->fFreezeDelay <= 0) {
            //unfreeze
            enemy->fFreezeDelay = 0;
        }

    } else if (!enemy->isAlive) {
        enemy->fDeathDelay += DELTA_TIME;

    } else {
        updateActive_enemy(enemy);
    }
    if (enemy->fDamagedCountdown > 0) {
        enemy->fDamagedCountdown -= DELTA_TIME;

        if (enemy->fDamagedCountdown < 0) {
            enemy->fDamagedCountdown = 0;

        }

    }

}
void updateActive_enemy(struct Enemy *enemy) {

    enemy->fLifetime += DELTA_TIME;

    switch (enemy->iType) {
        case id_alpha:
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

        case id_bravo:
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
        case id_charlie:
            //Charlie
            if (enemy->iLevel == 1) {
                enemy->fActiveTime += DELTA_TIME;
                enemy->x = enemy->orig_x + 100 * sin(enemy->fActiveTime * PI);
                enemy->y += enemy->vel_y * DELTA_TIME;

            } else if (enemy->iLevel == 2) {

                if (enemy->fChangeMovementCountdown > 0) {
                    enemy->fActiveTime += DELTA_TIME;

                    enemy->fChangeMovementCountdown -= DELTA_TIME;

                    enemy->x = enemy->orig_x + 250 * sin(enemy->fActiveTime * PI);
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

        case id_delta:
            updatePosition_enemy(enemy, enemy->iType, enemy->iLevel);
            break;

        case id_echo:
            //Echo
            if (enemy->fWaitCountdown > 0) {
                enemy->fWaitCountdown -= DELTA_TIME;
                if (enemy->fWaitCountdown <= 0) {
                    enemy->fWaitCountdown = 0;
                    setTargetPosition_enemy(enemy, (1 + (rand() % ((SCREEN_WIDTH / 64) - 2))) * 64, (1 + (rand() % ((SCREEN_HEIGHT / 64) - 2))) * 64);

                }
            } else if (enemy->iMoveToTarget) {

                if (getDistance(enemy->x, enemy->y, enemy->target_x, enemy->target_y) > sqrt(pow(enemy->vel_x, 2) + pow(enemy->vel_y, 2))) {
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

        case id_foxtrot:
            //Foxtrot

            if (enemy->fWaitCountdown > 0) {
                enemy->fWaitCountdown -= DELTA_TIME;
            } else {

                enemy->fChangeMovementCountdown = enemy->fChangeMovementCountdown - (1 * DELTA_TIME);
                if (enemy->fChangeMovementCountdown <= 0) {
                    int iRand = rand() % 4;
                    int iDirX = 0;
                    int iDirY = 0;
                    if (iRand == 0) {
                        iDirX = 1;
                    } else if (iRand == 1) {
                        iDirX = -1;
                    } else if (iRand == 2) {
                        iDirY = 1;
                    } else if (iRand == 3) {
                        iDirY = -1;
                    }

                    float fSpeed = 0.5;
                    if (enemy->iLevel == 1) {
                        fSpeed = 0.5;
                    } else if (enemy->iLevel == 2) {
                        fSpeed = 2;
                    }

                    enemy->vel_x = iDirX * fSpeed * UNIT_SIZE;
                    enemy->vel_y = iDirY * fSpeed * UNIT_SIZE;
                    enemy->fChangeMovementCountdown = 1 + (rand() % 2);

                    iRand = rand() % 2;
                    if (iRand == 0) {
                        if (enemy->iLevel == 1) {
                            enemy->fWaitCountdown = 1;
                        } else if (enemy->iLevel == 2) {
                            enemy->fWaitCountdown = 0.5;

                        }
                    }

                }
                if (enemy->x + enemy->vel_x * DELTA_TIME < 0 ||
                    enemy->x + enemy->width + enemy->vel_x * DELTA_TIME > SCREEN_WIDTH
                    ) {
                    enemy->vel_x *= -1;
                }
                if (enemy->y + enemy->vel_y * DELTA_TIME < 0 ||
                    enemy->y + enemy->height + enemy->vel_y * DELTA_TIME > SCREEN_HEIGHT
                    ) {
                    enemy->vel_y *= -1;
                }

                enemy->x += enemy->vel_x * DELTA_TIME;
                enemy->y += enemy->vel_y * DELTA_TIME;
            }
            break;
        case id_golf:
            //Golf
            update_enemy_golf(enemy);
            break;

        case id_hotel:
            //Hotel
            update_enemy_hotel(enemy);
            break;

        case id_india:
            //India
            update_enemy_india(enemy);
            break;
        case id_juliett:
            //Juliett
            update_enemy_juliett(enemy);
            break;
        case id_kilo:
            //Kilo
            update_enemy_kilo(enemy);
            break;
        case id_lima:
            //Lima
            update_enemy_lima(enemy);
            break;
        case id_mike:
            //Mike
            update_enemy_mike(enemy);
            break;
    }

    if (enemy->fShootDelay > 0) {
        enemy->fShootDelay -= DELTA_TIME;
        if (enemy->fShootDelay <= 0) {
            shoot_enemy(enemy);
            setShootDelay_enemy(enemy);
        }
    }
    //wrap enemy around top/bottom of screen
    if (enemy->y > SCREEN_HEIGHT) {
        enemy->y -= SCREEN_HEIGHT + enemy->height;
    } else if (enemy->y + enemy->height < 0) {
        enemy->y += SCREEN_HEIGHT + enemy->height;

    }
}

void updatePosition_enemy(struct Enemy *enemy, int iType, int iLevel) {
    switch (iType) {
        case 3:
            //Delta
            enemy->x = enemy->orig_x + (128 * cos(enemy->fLifetime * PI));
            enemy->y = enemy->orig_y + (128 * sin(enemy->fLifetime * PI));
            break;

    }
}

void draw_enemy(struct Enemy *enemy) {
    SDL_Rect pos;
    SDL_Texture *img = NULL;

    int iSpriteIndex = ((int)(enemy->fLifetime * 2)) % 2; //change sprite every 0.5 secondsDL_SetTextureColorMod
//    if (enemy->isAlive) {
    pos.x = enemy->x;
    pos.y = enemy->y;
    pos.w = enemy->width;
    pos.h = enemy->height;
    if (enemy->fIntroDelay > 0) {
        img = imgEnemyWarp;
    } else {

        switch (enemy->iType) {
            //Alpha
            case id_alpha:
                if (iSpriteIndex == 0) {
                    if (enemy->iLevel == 1) {
                        img = imgEnemyAlpha_L1_00;
                    } else if (enemy->iLevel == 2) {
                        img = imgEnemyAlpha_L2_00;
                    }
                } else if (iSpriteIndex == 1) {
                    if (enemy->iLevel == 1) {
                        img = imgEnemyAlpha_L1_01;

                    } else if (enemy->iLevel == 2) {
                        img = imgEnemyAlpha_L2_01;

                    }
                }

                break;
                //Bravo
            case id_bravo:

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
                //Charlie
            case id_charlie:

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
                //Delta
            case id_delta:

                if (iSpriteIndex == 0) {
                    img = imgEnemyDelta_L1_00;

                } else if (iSpriteIndex == 1) {
                    img = imgEnemyDelta_L1_01;

                }

                break;

                //Echo
            case id_echo:
                if (iSpriteIndex == 0) {
                    if (enemy->fWaitCountdown > 0) {
                        img = imgEnemyEcho_L1_00;
                    } else {
                        img = imgEnemyEcho_L1_02;
                    }
                } else if (iSpriteIndex == 1) {
                    if (enemy->fWaitCountdown > 0) {
                        img = imgEnemyEcho_L1_01;
                    } else {
                        img = imgEnemyEcho_L1_03;
                    }
                }

                break;

                //Foxtrot
            case id_foxtrot:
                if (iSpriteIndex == 0) {
                    if (enemy->iLevel == 1) {
                        img = imgEnemyFoxtrot_L1_00;
                    } else if (enemy->iLevel == 2) {
                        img = imgEnemyFoxtrot_L2_00;
                    }
                } else if (iSpriteIndex == 1) {
                    if (enemy->iLevel == 1) {
                        img = imgEnemyFoxtrot_L1_01;

                    } else if (enemy->iLevel == 2) {
                        img = imgEnemyFoxtrot_L2_01;

                    }
                }

                break;

                //Golf
            case id_golf:
                //draw_enemy_golf(enemy);
                img = getTexture_enemy_golf(enemy);

                break;

                //Hotel
            case id_hotel:
                img = getTexture_enemy_hotel(enemy);
                break;

                //India
            case id_india:
                img = getTexture_enemy_india(enemy);
                break;
                //Juliett
            case id_juliett:
                img = getTexture_enemy_juliett(enemy);
                break;

                //Kilo
            case id_kilo:
                img = getTexture_enemy_kilo(enemy);
                break;

                //Lima
            case id_lima:
                img = getTexture_enemy_lima(enemy);
                break;

                //Mike
            case id_mike:
                img = getTexture_enemy_mike(enemy);
                break;

        }
    }

    if (img != NULL) {
        SDL_SetTextureAlphaMod(img, 0xFF);
        if (enemy->fIntroDelay > 0) {
            SDL_RenderCopyEx(renderer, img, NULL, &pos, enemy->fIntroDelay * 720, NULL, SDL_FLIP_NONE);
        } else if (!enemy->isAlive) {
            draw_explosion_enemy(enemy, img);
            /*

            SDL_Rect rectParts;
            float fBreakApartSpeed = 2;
//				int iAlpha = 0xFF - (enemy->fDeathDelay * 256 / enemy->fMaxDeathDelay);
                float fDeathPercent = enemy->fDeathDelay / enemy->fMaxDeathDelay;
                //full transperancy for half of death delay, then fade linearly
                int iAlpha = 255;
                if (fDeathPercent > 0.5) {
                    iAlpha = 256 * (-((fDeathPercent - 0.5) / 0.5) + 1);
//					iAlpha = 256 * (-(fDeathPercent) + 1);
                }
                if (iAlpha < 0) {
                    iAlpha = 0;
                }

                float fAngle = enemy->fDeathDelay * 180;
                SDL_SetTextureAlphaMod(img, iAlpha);

                int img_w, img_h;
                SDL_QueryTexture(img, NULL, NULL, &img_w, &img_h);
                //upper left part
                pos.x = enemy->x + (enemy->width / 2) - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
                pos.y = enemy->y + (enemy->height / 2) - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
                pos.w = enemy->width / 2;
                pos.h = enemy->height / 2;

                rectParts.x = 0;
                rectParts.y = 0;
                rectParts.w = img_w / 2;
                rectParts.h = img_h / 2;

//				SDL_RenderCopyEx(renderer, img, &rectParts, &pos, 0, NULL, SDL_FLIP_NONE);
                SDL_RenderCopyEx(renderer, img, &rectParts, &pos, -fAngle, NULL, SDL_FLIP_NONE);
                //upper right part
                pos.x = enemy->x + (enemy->width / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
                pos.y = enemy->y + (enemy->height / 2) - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
                pos.w = enemy->width / 2;
                pos.h = enemy->height / 2;

                rectParts.x = img_w / 2;
                rectParts.y = 0;
                rectParts.w = img_w / 2;
                rectParts.h = img_h / 2;

                SDL_RenderCopyEx(renderer, img, &rectParts, &pos, fAngle, NULL, SDL_FLIP_NONE);
                //lower left part
                pos.x = enemy->x + (enemy->width / 2) - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
                pos.y = enemy->y + (enemy->height / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
                pos.w = enemy->width / 2;
                pos.h = enemy->height / 2;

                rectParts.x = 0;
                rectParts.y = img_h / 2;
                rectParts.w = img_w / 2;
                rectParts.h = img_h / 2;

                SDL_RenderCopyEx(renderer, img, &rectParts, &pos, -fAngle, NULL, SDL_FLIP_NONE);

                //lower right part
                pos.x = enemy->x + (enemy->width / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
                pos.y = enemy->y + (enemy->height / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
                pos.w = enemy->width / 2;
                pos.h = enemy->height / 2;

                rectParts.x = img_w / 2;
                rectParts.y = img_h / 2;
                rectParts.w = img_w / 2;
                rectParts.h = img_h / 2;

                SDL_RenderCopyEx(renderer, img, &rectParts, &pos, fAngle, NULL, SDL_FLIP_NONE);
*/
        } else {

            if (enemy->iType != id_mike) {//enemy mike

                if (enemy->fDamagedCountdown > 0 || enemy->iHealth <= 0) {
                    SDL_SetTextureColorMod(img, 255, 0, 0);
                } else {
                    SDL_SetTextureColorMod(img, 255, 255, 255);

                }
            }

            //draw enemy sprite
            SDL_RenderCopy(renderer, img, NULL, &pos);
            //draw frozen block
            if (enemy->fFreezeDelay > 0) {
                int iFreezeTransperancy = 64 + (128 * enemy->fFreezeDelay / enemy->fMaxFreezeDelay);

                //					if (iFreezeTransperancy > 192) {
                //						iFreezeTransperancy = 192;
                //					}
                SDL_Rect rectFreeze = { enemy->x, enemy->y, enemy->width, enemy->height };
                SDL_SetRenderDrawColor(renderer, 97, 162, 255, iFreezeTransperancy);
                SDL_RenderFillRect(renderer, &rectFreeze);

            }

        }
    }

    //  }

}

void draw_explosion_enemy(struct Enemy *enemy, SDL_Texture *img) {
    SDL_Rect pos;
    SDL_Rect rectParts;
    float fBreakApartSpeed = 2;
    float fDeathPercent = enemy->fDeathDelay / enemy->fMaxDeathDelay;

    //full transperancy for half of death delay, then fade linearly
    int iAlpha = 255;
    if (fDeathPercent > 0.5) {
        iAlpha = 256 * (-((fDeathPercent - 0.5) / 0.5) + 1);
    }
    if (iAlpha < 0) {
        iAlpha = 0;
    }

    float fAngle = enemy->fDeathDelay * 180;
    SDL_SetTextureAlphaMod(img, iAlpha);

    int img_w, img_h;
    SDL_QueryTexture(img, NULL, NULL, &img_w, &img_h);
    //upper left part
//			    pos.x = enemy->x + (enemy->width / 2) - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
//				pos.y = enemy->y + (enemy->height / 2) - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);

    pos.x = enemy->x + 0 - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.y = enemy->y + 0 - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.w = enemy->width / 2;
    pos.h = enemy->height / 2;

    rectParts.x = 0;
    rectParts.y = 0;
    rectParts.w = img_w / 2;
    rectParts.h = img_h / 2;

    SDL_RenderCopyEx(renderer, img, &rectParts, &pos, -fAngle, NULL, SDL_FLIP_NONE);
    //upper right part
//			    pos.x = enemy->x + (enemy->width / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
//				pos.y = enemy->y + (enemy->height / 2) - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.x = enemy->x + (enemy->width / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.y = enemy->y + 0 - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.w = enemy->width / 2;
    pos.h = enemy->height / 2;

    rectParts.x = img_w / 2;
    rectParts.y = 0;
    rectParts.w = img_w / 2;
    rectParts.h = img_h / 2;

    SDL_RenderCopyEx(renderer, img, &rectParts, &pos, fAngle, NULL, SDL_FLIP_NONE);
    //lower left part
//			    pos.x = enemy->x + (enemy->width / 2) - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
//				pos.y = enemy->y + (enemy->height / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.x = enemy->x + 0 - (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.y = enemy->y + (enemy->height / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.w = enemy->width / 2;
    pos.h = enemy->height / 2;

    rectParts.x = 0;
    rectParts.y = img_h / 2;
    rectParts.w = img_w / 2;
    rectParts.h = img_h / 2;

    SDL_RenderCopyEx(renderer, img, &rectParts, &pos, -fAngle, NULL, SDL_FLIP_NONE);

    //lower right part
//			    pos.x = enemy->x + (enemy->width / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
//				pos.y = enemy->y + (enemy->height / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.x = enemy->x + (enemy->width / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.y = enemy->y + (enemy->height / 2) + (enemy->fDeathDelay * fBreakApartSpeed * UNIT_SIZE);
    pos.w = enemy->width / 2;
    pos.h = enemy->height / 2;

    rectParts.x = img_w / 2;
    rectParts.y = img_h / 2;
    rectParts.w = img_w / 2;
    rectParts.h = img_h / 2;

    SDL_RenderCopyEx(renderer, img, &rectParts, &pos, fAngle, NULL, SDL_FLIP_NONE);

}

void shoot_enemy(struct Enemy *enemy) {
    struct Bullet *bullet;
    float fDistance;

    if (enemy != NULL && (enemy->fShootDelay <= 0) && enemy->isAlive) {

        switch (enemy->iType) {
            case id_alpha:
            case id_bravo:
                bullet = malloc(sizeof(struct Bullet));

                init_bullet(bullet, enemy->x + enemy->width / 2, enemy->y, 0);

                bullet->vel_y = 5;
                bullet->iHitsPlayer = TRUE;

                add_node(&listBullet, bullet);
                Mix_PlayChannel(-1, soundEnemyShoot, 0);
                break;

            case id_delta:
                bullet = malloc(sizeof(struct Bullet));

                init_bullet(bullet, enemy->x + enemy->width / 2, enemy->y, 0);

                fDistance = getDistance(ship->x, ship->y, bullet->x, bullet->y);
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
    switch (enemy->iType) {
        case id_alpha:
            enemy->fShootDelay = 1 + ((rand() % 50) * 0.1);  //between 1 and 6 seconds
            break;
        case id_bravo:
            if (enemy->iLevel == 1) {
                enemy->fShootDelay = 5 + ((rand() % 50) * 0.1); //between 5 and 10 seconds
            } else if (enemy->iLevel == 2) {
                enemy->fShootDelay = 1; //between 2 and 4 seconds

            }
            break;
        case id_delta:
            enemy->fShootDelay = 2; //2 seconds
            break;
    }

}

void damage_enemy(struct Enemy *enemy, int iDamageAmount) {
    int iTotalDamage = 0;
    int iCheckDestroy = TRUE;

    if (enemy->iType == id_echo) {
        if (enemy->fWaitCountdown > 0) {
            iTotalDamage = iDamageAmount; //can only damage this enemy if it's waiting with eye open
        }
    } else if (enemy->iType == id_golf) {
        damage_enemy_golf(enemy, iDamageAmount);
        iCheckDestroy = FALSE;
    } else if (enemy->iType == id_hotel) {
        damage_enemy_hotel(enemy, iDamageAmount);
        iCheckDestroy = FALSE;
    } else if (enemy->iType == id_india) {
        damage_enemy_india(enemy, iDamageAmount);
        iCheckDestroy = FALSE;

    } else if (enemy->iType == id_juliett) {
        damage_enemy_juliett(enemy, iDamageAmount);
        iCheckDestroy = FALSE;
    } else if (enemy->iType == id_kilo) {
        damage_enemy_kilo(enemy, iDamageAmount);
        iCheckDestroy = FALSE;
    } else if (enemy->iType == id_lima) {
        damage_enemy_lima(enemy, iDamageAmount);
        iCheckDestroy = FALSE;
    } else if (enemy->iType == id_mike) {
        damage_enemy_mike(enemy, iDamageAmount);
        iCheckDestroy = FALSE;

    } else {
        iTotalDamage = iDamageAmount;

    }
    if (iCheckDestroy) {
        if (iTotalDamage > 0) {
            enemy->iHealth -= iTotalDamage;
            if (enemy->iHealth <= 0) {
                //			enemy->isAlive = FALSE;
                kill_enemy(enemy);

            } else {
                Mix_PlayChannel(-1, soundEnemyHit, 0);

            }
            enemy->fDamagedCountdown = 0.2;
        } else {
            Mix_PlayChannel(-1, soundEnemyShield, 0);

        }
    }
}

void freeze_enemy(struct Enemy *enemy, int iFreezeLevel, int iDamageAmount) {
    int iNewFreezeDelay;
    int iCanFreeze;
    iCanFreeze = TRUE;
    if (enemy->iType == 6) {
        //Golf
        iCanFreeze = getCanFreeze_enemy_golf(enemy);
    }
    if (iCanFreeze) {
        if (enemy->fFreezeDelay > 0) {
            //if the enemy is already froze, then damage the enemy
            damage_enemy(enemy, iDamageAmount);
        }
        //don't need to reset the freeze dealy, if the enemy is already dead
        //refreezing a dead enemy will cause weird things to happen in the death animation
        if (enemy->isAlive) {
            switch (iFreezeLevel) {
                case 0:
                    iNewFreezeDelay = 1;
                    break;
                case 1:
                    iNewFreezeDelay = 4;
                    break;
                case 2:
                    iNewFreezeDelay = 20;
                    break;
            }

            if (enemy->fFreezeDelay < iNewFreezeDelay) {
                enemy->fFreezeDelay = iNewFreezeDelay;
                enemy->fMaxFreezeDelay = iNewFreezeDelay;
            }
        }
    } else {
        damage_enemy(enemy, iDamageAmount);

    }
}

//kill_enemy set the enemy to not alive, but does not free the associated memory
//sets fDeathDelay to give time to display the explosion animation
void kill_enemy(struct Enemy *enemy) {
    enemy->isAlive = FALSE;
    enemy->fDeathDelay = 0;
    enemy->fFreezeDelay = 0;
    stats->iScore += enemy->iPoints;

    Mix_PlayChannel(-1, soundEnemyDead, 0);
    if (enemy->hasDrop) {
        int iType;
        iType = rand() % 4;

        struct Powerup *powerup = malloc(sizeof(struct Powerup));
        init_powerup(powerup, enemy->x, enemy->y, iType);
        add_node(&listPowerup, powerup);
    }

    /*
                    struct Explosion *explosion = malloc(sizeof(struct Explosion));
                    init_explosion(explosion, enemy->x + (enemy->width / 2), enemy->y + (enemy->height / 2), enemy->width / 2);
                    explosion->c.r = 255;
                    explosion->c.g = 128;
                    explosion->c.b = 128;

                    add_node(&listExplosion, explosion);
    */

    updateDisplayText();

}
void setTargetPosition_enemy(struct Enemy *enemy, float x, float y) {
    float fSpeed = 1;
    enemy->target_x = x;
    enemy->target_y = y;

    float fDistanceX = enemy->target_x - enemy->x;
    float fDistanceY = enemy->target_y - enemy->y;
    float fDistance = sqrt(pow(fDistanceX, 2) + pow(fDistanceY, 2));
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

float getCenterX_enemy(struct Enemy *enemy) {
    return enemy->x + (enemy->width / 2);
}

float getCenterY_enemy(struct Enemy *enemy) {
    return enemy->y + (enemy->height / 2);
}

int canDestroy_enemy(struct Enemy *enemy) {
    int iCanDelete = FALSE;
    if (!enemy->isAlive && enemy->fDeathDelay > enemy->fMaxDeathDelay) {
        iCanDelete = TRUE;

    }

    return iCanDelete;
}

//destroy_enemy - frees the memory associated with the enemy
void destroy_enemy(struct Enemy *enemy) {

    switch (enemy->iType) {
        //Kilo
        case id_kilo:
            destroy_enemy_kilo(enemy);

            break;

    }
}

