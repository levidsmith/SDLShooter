#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "globals.h"
#include "enemy.h"
#include "enemy_golf.h"

extern SDL_Renderer *renderer;

extern Mix_Chunk *soundEnemyHit;
extern Mix_Chunk *soundEnemyDead;
extern Mix_Chunk *soundEnemyShield;

extern SDL_Texture *imgEnemyGolf_L1_00;
extern SDL_Texture *imgEnemyGolf_L1_01;
extern SDL_Texture *imgEnemyGolf_L1_02;
extern SDL_Texture *imgEnemyGolf_L1_03;
extern SDL_Texture *imgEnemyGolf_L1_04;
extern SDL_Texture *imgEnemyGolf_L1_05;

extern SDL_Texture *imgEnemyGolf_L2_00;
extern SDL_Texture *imgEnemyGolf_L2_01;
extern SDL_Texture *imgEnemyGolf_L2_02;
extern SDL_Texture *imgEnemyGolf_L2_03;
extern SDL_Texture *imgEnemyGolf_L2_04;
extern SDL_Texture *imgEnemyGolf_L2_05;

void init_enemy_golf(struct Enemy *enemy) {
    enemy->subtype = malloc(sizeof(struct EnemyGolf));
    struct EnemyGolf *enemygolfhead = (struct EnemyGolf *) enemy->subtype;
    enemygolfhead->previous = NULL;
    enemygolfhead->iIsHead = TRUE;

    enemygolfhead->iFoo = 42;
    printf("init_enemy_golf: %d\n", enemygolfhead->iFoo);

    int iSegments = 0;
    if (enemy->iLevel == 1) {
        enemy->iHealth = 1;
        iSegments = 3 + (rand() % 3);
    } else if (enemy->iLevel == 2) {
        enemy->iHealth = 2;
        iSegments = 5 + (rand() % 3);
    }

    if (enemy->iLevel == 1) {
        enemy->iHealth = 1;
        iSegments = 3 + (rand() % 3);
        enemy->vel_x = 2;
    } else if (enemy->iLevel == 2) {
        enemy->iHealth = 2;
        iSegments = 4 + (rand() % 2);
        enemy->vel_x = 4 + (rand() % 2);
    }

    int iVelDir = rand() % 2;
    if (iVelDir == 0) {
        enemy->vel_x *= -1;
    }

    enemy->hasDrop = TRUE;
    struct Enemy *enemyPrevious = enemy;
    struct EnemyGolf *enemygolfprevious = (struct EnemyGolf *) enemy->subtype;
    int i;
    for (i = 0; i < iSegments; i++) {
        struct Enemy *enemyTail1 = malloc(sizeof(struct Enemy));
        float x_pos;
        if (enemy->vel_x > 0) {
            x_pos = enemyPrevious->x - UNIT_SIZE;
        } else {
            x_pos = enemyPrevious->x + UNIT_SIZE;
        }

        init_enemy(enemyTail1, x_pos, enemyPrevious->y, enemyPrevious->iType, enemyPrevious->iLevel, FALSE);

        if (enemyTail1->iLevel == 1) {
            enemyTail1->iHealth = 1;
        } else if (enemyTail1->iLevel == 2) {
            enemyTail1->iHealth = 2;
        }

        //		init_enemy_golf_tail(enemyTail1, enemy);

        struct EnemyGolf *enemygolftail1 = malloc(sizeof(struct EnemyGolf));
        enemygolftail1->iFoo = 42 + i;
        enemygolftail1->previous = NULL;
        enemygolftail1->next = NULL;
        enemygolftail1->iIsHead = FALSE;

        enemyTail1->subtype = enemygolftail1;
        enemygolfprevious->next = enemyTail1;
        enemygolftail1->previous = enemyPrevious;

        enemyPrevious = enemyTail1;
        enemygolfprevious = (struct EnemyGolf *) enemyTail1->subtype;

    }

}

/*
void init_enemy_golf_tail(struct Enemy *enemy, struct Enemy *previous) {

}*/
void update_enemy_golf(struct Enemy *enemy) {

    if (enemy != NULL && enemy->subtype != NULL) {
        struct EnemyGolf *enemygolf = (struct EnemyGolf *) enemy->subtype;

        if (enemygolf->iIsHead) {
            updateMinMaxValues(enemy);
            //			printf("x_min: %f, x_max: %f, y_min: %f, y_max: %f\n", enemygolf->x_min, enemygolf->x_max, enemygolf->y_min, enemygolf->y_max);

            if (enemygolf->x_min > SCREEN_WIDTH) {
                moveAll(enemy, -enemygolf->x_max, 0);
            } else if (enemygolf->x_max < 0) {
                moveAll(enemy, SCREEN_WIDTH - enemygolf->x_min, 0);
            }
        }
        if (enemygolf->previous != NULL) {
            //enemy->x = enemygolf->previous->x - UNIT_SIZE;
            float x_diff = enemygolf->previous->x - enemy->x;
            float y_diff = enemygolf->previous->y - enemy->y;
            float fMag = sqrt(pow(x_diff, 2) + pow(y_diff, 2));
            float fAngleX = acos(x_diff / fMag);
            float fAngleY = asin(y_diff / fMag);
            //			printf("current x %f, y %f, previous x %f, y %f\n", enemy->x, enemy->y, enemygolf->previous->x, enemygolf->previous->y);
            //			printf("x_diff: %f, y_diff: %f, AngleX: %f, AngleY: %f, fMag: %f\n", x_diff, y_diff, fAngleX, fAngleY, fMag);

            if (fMag > 1 * UNIT_SIZE) {
                enemy->x += cos(fAngleX) * (fMag - UNIT_SIZE);
                enemy->y += sin(fAngleY) * (fMag - UNIT_SIZE);
            }

        } else {
            enemy->x += enemy->vel_x * UNIT_SIZE * DELTA_TIME;
            //enemy->y = enemy->orig_y + (UNIT_SIZE * sin(enemy->fActiveTime * PI));
            float fAmplitude = 2;
            float fPeriod = 0.5;

            if (enemy->iLevel == 1) {
                fAmplitude = 2;
                fPeriod = 0.5;
            } else if (enemy->iLevel == 2) {
                fAmplitude = 3;
                fPeriod = 0.25;
            }
            enemy->y = enemy->orig_y + (UNIT_SIZE * fAmplitude * sin(enemy->fLifetime * PI * fPeriod));
        }
    }
    //	printf("end update_enemy_golf\n");

}
//void draw_enemy_golf(struct Enemy *enemy) {
SDL_Texture *getTexture_enemy_golf(struct Enemy *enemy) {
    SDL_Texture *img = NULL;
    int iIsTailDead;
    if (enemy != NULL) {
        struct EnemyGolf *enemygolf = (struct EnemyGolf *) enemy->subtype;
        //		printf("draw_enemy_golf id: %d\n", enemygolf->iFoo);
        int iSpriteIndex = ((int)(enemy->fLifetime * 2)) % 2; //change sprite every 0.5 seconds
        SDL_Rect rect = { enemy->x, enemy->y, enemy->width, enemy->height };

        if (enemygolf->iIsHead) {
            iIsTailDead = getIsTailDead(enemy);

            if (iSpriteIndex == 0) {
                if (enemy->iLevel == 1) {
                    if (!iIsTailDead) {
                        img = imgEnemyGolf_L1_00;
                    } else {
                        img = imgEnemyGolf_L1_02;
                    }
                } else if (enemy->iLevel == 2) {
                    if (!iIsTailDead) {
                        img = imgEnemyGolf_L2_00;
                    } else {
                        img = imgEnemyGolf_L2_02;
                    }
                }
            } else if (iSpriteIndex == 1) {
                if (enemy->iLevel == 1) {
                    if (!iIsTailDead) {
                        img = imgEnemyGolf_L1_01;
                    } else {
                        img = imgEnemyGolf_L1_03;
                    }

                } else if (enemy->iLevel == 2) {
                    if (!iIsTailDead) {
                        img = imgEnemyGolf_L2_01;
                    } else {
                        img = imgEnemyGolf_L2_03;
                    }

                }
            }
        } else {
            if (iSpriteIndex == 0) {
                if (enemy->iLevel == 1) {
                    img = imgEnemyGolf_L1_04;
                } else if (enemy->iLevel == 2) {
                    img = imgEnemyGolf_L2_04;
                }
            } else if (iSpriteIndex == 1) {
                if (enemy->iLevel == 1) {
                    img = imgEnemyGolf_L1_05;

                } else if (enemy->iLevel == 2) {
                    img = imgEnemyGolf_L2_05;

                }
            }
        }

        /*
                        if (enemy->fDamagedCountdown > 0 || enemy->iHealth <= 0) {
                                SDL_SetTextureColorMod(img, 255, 0, 0);
                        } else {
                            SDL_SetTextureColorMod(img, 255, 255, 255);

                        }
                        */

                        //draw enemy sprite
        SDL_Rect pos;
        pos.x = enemy->x;
        pos.y = enemy->y;
        pos.w = enemy->width;
        pos.h = enemy->height;
        //				SDL_RenderCopy(renderer, img, NULL, &pos);

                /*
                if (enemy->iHealth > 0) {
                    if (enemygolf->iIsHead) {
                        if (getIsTailDead(enemy)) {
                            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                        } else {
                            SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
                        }
                    } else {
                        SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
                    }
                } else {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                }

                SDL_RenderFillRect(renderer, &rect);
                */

                //draw frozen block
        if (enemy->fFreezeDelay > 0) {
            int iFreezeTransperancy = 64 + (128 * enemy->fFreezeDelay / enemy->fMaxFreezeDelay);
            SDL_Rect rectFreeze = { enemy->x, enemy->y, enemy->width, enemy->height };
            SDL_SetRenderDrawColor(renderer, 97, 162, 255, iFreezeTransperancy);
            SDL_RenderFillRect(renderer, &rectFreeze);

        }
    }

    return img;

}

void damage_enemy_golf(struct Enemy *enemy, int iDamageAmount) {
    struct EnemyGolf *enemygolf = (struct EnemyGolf *) enemy->subtype;
    if (enemygolf->iIsHead) {
        int iTailIsDead = TRUE;

        //		enemy->isAlive = FALSE;
        iTailIsDead = getIsTailDead(enemy);
        if (iTailIsDead) {
            enemy->iHealth -= iDamageAmount;

            if (enemy->iHealth <= 0) {
                kill_enemy(enemy);
                //delete the head and all tail segments
                struct Enemy *current = ((struct EnemyGolf *) enemy->subtype)->next;
                while (current != NULL) {

                    kill_enemy(current);

                    current = ((struct EnemyGolf *)current->subtype)->next;
                }

                Mix_PlayChannel(-1, soundEnemyDead, 0);

            } else {
                Mix_PlayChannel(-1, soundEnemyHit, 0);
                enemy->fDamagedCountdown = 0.2;
            }

        } else {
            Mix_PlayChannel(-1, soundEnemyShield, 0);

        }

    } else {
        if (enemy->iHealth > 0) {
            enemy->iHealth -= iDamageAmount;
            enemy->fDamagedCountdown = 0.2;

            Mix_PlayChannel(-1, soundEnemyHit, 0);
        } else {
            //segment is already destroyed
            Mix_PlayChannel(-1, soundEnemyShield, 0);

        }
    }

}

int getIsTailDead(struct Enemy *enemy) {
    int iIsTailDead = TRUE;
    if (enemy != NULL && enemy->subtype != NULL) {
        struct Enemy *current = ((struct EnemyGolf *) enemy->subtype)->next;
        while (current != NULL) {
            if (current->iHealth > 0) {
                iIsTailDead = FALSE;
            }
            if (current->subtype != NULL) {
                current = ((struct EnemyGolf *)current->subtype)->next;
            } else {
                current = NULL;
            }
        }

    }
    return iIsTailDead;

}

void updateMinMaxValues(struct Enemy *enemy) {
    struct EnemyGolf *enemygolfhead;

    if (enemy != NULL && enemy->subtype != NULL) {
        enemygolfhead = (struct EnemyGolf *) enemy->subtype;
        enemygolfhead->x_min = enemy->x;
        enemygolfhead->x_max = enemy->x + enemy->width;
        enemygolfhead->y_min = enemy->y;
        enemygolfhead->y_max = enemy->y + enemy->height;

        struct Enemy *current = enemygolfhead->next;
        struct EnemyGolf *enemygolf;

        while (current != NULL) {
            enemygolf = (struct EnemyGolf *) current->subtype;

            if (current->x < enemygolfhead->x_min) {
                enemygolfhead->x_min = current->x;
            } else if (current->x + current->width > enemygolfhead->x_max) {
                enemygolfhead->x_max = current->x + current->width;
            }
            if (current->subtype != NULL) {
                current = ((struct EnemyGolf *)current->subtype)->next;
            } else {
                current = NULL;
            }
        }

    }

}

void moveAll(struct Enemy *enemy, float x_move, float y_move) {
    //assumes that the head is passed in
    struct EnemyGolf *enemygolf;

    if (enemy != NULL && enemy->subtype != NULL) {
        enemy->x += x_move;
        enemy->y += y_move;

        enemygolf = (struct EnemyGolf *) enemy->subtype;
        struct Enemy *current = enemygolf->next;

        while (current != NULL) {
            enemygolf = (struct EnemyGolf *) current->subtype;

            current->x += x_move;
            current->y += y_move;
            if (current->subtype != NULL) {
                current = ((struct EnemyGolf *)current->subtype)->next;
            } else {
                current = NULL;
            }
        }

    }
}

int getCanFreeze_enemy_golf(struct Enemy *enemy) {
    int iCanFreeze = FALSE;
    struct EnemyGolf *enemygolf;
    enemygolf = ((struct EnemyGolf *)enemy->subtype);
    if (enemygolf->iIsHead && getIsTailDead(enemy)) {
        iCanFreeze = TRUE;

    }

    return iCanFreeze;
}
