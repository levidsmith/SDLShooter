//2020 Levi D. Smith
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "globals.h"
#include "enemy.h"
#include "enemy_kilo.h"
#include "screen_game.h"

extern SDL_Texture *imgEnemyKilo_L1_00;
extern SDL_Texture *imgEnemyKilo_L1_01;
extern SDL_Texture *imgEnemyKilo_L2_00;
extern SDL_Texture *imgEnemyKilo_L2_01;

//SDL_Texture *imgNumberValue;
SDL_Texture *imgNumberZero;
SDL_Texture *imgNumberOne;

extern TTF_Font *fontDefault;

void init_enemy_kilo(struct Enemy *enemy) {

    SDL_Color colorGreenText = {0, 128, 0};
    generateTextTexture(&imgNumberZero, "0", colorGreenText, fontDefault);
    generateTextTexture(&imgNumberOne, "1", colorGreenText, fontDefault);

    
    struct EnemyKiloHead *enemykilohead = malloc(sizeof(struct EnemyKiloHead));
    int iTargetValue = (rand() % 14) + 1;
    enemykilohead->iTargetValue = iTargetValue;
    enemy->subtype = enemykilohead;

    char strText[8];
    sprintf(strText, "%d", iTargetValue);
    SDL_Color colorRedText = {255, 0, 0};
    SDL_Texture *imgNumberValue;
    generateTextTexture(&imgNumberValue, strText, colorRedText, fontDefault);
    enemykilohead->imgNumberValue = imgNumberValue;

    
    struct Enemy *enemyPart;
    struct EnemyKiloBody *enemykilobody;
    
    
    int iWidth = 64;
    int iHeight = 64;
    //first part
    enemyPart = malloc(sizeof(struct Enemy));
    init_enemy(enemyPart, enemy->x - (iWidth * 1.5), enemy->y + 64, 10, 2, FALSE);
    enemykilobody = malloc(sizeof(struct EnemyKiloBody));
    enemykilobody->head = enemy;
    enemyPart->subtype = enemykilobody;
    enemyPart->width = iWidth;
    enemyPart->height = iHeight;
    enemykilohead->body[0] = enemyPart;

    //second part
    enemyPart = malloc(sizeof(struct Enemy));
    init_enemy(enemyPart, enemy->x - (iWidth * 0.5), enemy->y + 64, 10, 2, FALSE);
    enemykilobody = malloc(sizeof(struct EnemyKiloBody));
    enemykilobody->head = enemy;
    enemyPart->subtype = enemykilobody;
    enemyPart->width = iWidth;
    enemyPart->height = iHeight;
    enemykilohead->body[1] = enemyPart;

    //third part
    enemyPart = malloc(sizeof(struct Enemy));
    init_enemy(enemyPart, enemy->x + (iWidth * 0.5), enemy->y + 64, 10, 2, FALSE);
    enemykilobody = malloc(sizeof(struct EnemyKiloBody));
    enemykilobody->head = enemy;
    enemyPart->subtype = enemykilobody;
    enemyPart->width = iWidth;
    enemyPart->height = iHeight;
    enemykilohead->body[2] = enemyPart;

    //fourth part
    enemyPart = malloc(sizeof(struct Enemy));
    init_enemy(enemyPart, enemy->x + (iWidth * 1.5), enemy->y + 64, 10, 2, FALSE);
    enemykilobody = malloc(sizeof(struct EnemyKiloBody));
    enemykilobody->head = enemy;
    enemyPart->subtype = enemykilobody;
    enemyPart->width = iWidth;
    enemyPart->height = iHeight;
    enemykilohead->body[3] = enemyPart;

    
}

void update_enemy_kilo(struct Enemy *enemy) {

}

void damage_enemy_kilo(struct Enemy *enemy, int iDamageAmount) {
    if (enemy->iLevel == 2) {
        struct EnemyKiloBody *enemykilobody = (struct EnemyKiloBody *) enemy->subtype;
        enemykilobody->iValue--;
        if (enemykilobody->iValue < 0) {
            enemykilobody->iValue = 1;
        }
        check_destroy_enemy_kilo(enemykilobody->head);
    }
    
}

void check_destroy_enemy_kilo(struct Enemy *enemy) {
    int iAttackValue = 0;
    
    struct EnemyKiloHead *enemykilohead = (struct EnemyKiloHead *) enemy->subtype;
    struct Enemy *enemyPart1 = (struct Enemy *) enemykilohead->body[0];
    struct Enemy *enemyPart2 = (struct Enemy *) enemykilohead->body[1];
    struct Enemy *enemyPart3 = (struct Enemy *) enemykilohead->body[2];
    struct Enemy *enemyPart4 = (struct Enemy *) enemykilohead->body[3];
    struct EnemyKiloBody *enemykilobody1 = (struct EnemyKiloBody *) enemyPart1->subtype;
    struct EnemyKiloBody *enemykilobody2 = (struct EnemyKiloBody *) enemyPart2->subtype;
    struct EnemyKiloBody *enemykilobody3 = (struct EnemyKiloBody *) enemyPart3->subtype;
    struct EnemyKiloBody *enemykilobody4 = (struct EnemyKiloBody *) enemyPart4->subtype;
    
    iAttackValue = (enemykilobody1->iValue * 8) + (enemykilobody2->iValue * 4) +
    (enemykilobody3->iValue * 2) + (enemykilobody4->iValue * 1);
    
    if (iAttackValue == enemykilohead->iTargetValue) {
        destroy_enemy(enemyPart1);
        destroy_enemy(enemyPart2);
        destroy_enemy(enemyPart3);
        destroy_enemy(enemyPart4);
        destroy_enemy(enemy);

    }
    
    
    
}

SDL_Texture *getTexture_enemy_kilo(struct Enemy *enemy) {
    SDL_Texture *img = NULL;
    
    
       int iSpriteIndex = ((int) (enemy->fLifetime * 2)) % 2; //change sprite every 0.5 seconds
       
       
       if (iSpriteIndex == 0) {
           if (enemy->iLevel == 1) {
               img = imgEnemyKilo_L1_00;
           } else if (enemy->iLevel == 2) {
               img = imgEnemyKilo_L2_00;
           }
     } else if (iSpriteIndex == 1) {
         if (enemy->iLevel == 1) {
             img = imgEnemyKilo_L1_01;
         } else if (enemy->iLevel == 2) {
             img = imgEnemyKilo_L2_01;

         }
     }
    
    if (enemy->iLevel == 1) {
        struct EnemyKiloHead *enemykilohead = (struct EnemyKiloHead *) enemy->subtype;
      img = enemykilohead->imgNumberValue;
    } else {
        struct EnemyKiloBody *enemykilobody = (struct EnemyKiloBody *) enemy->subtype;
        if (enemykilobody->iValue == 0) {
            img = imgNumberZero;

        } else if (enemykilobody->iValue == 1) {
            img = imgNumberOne;

        }
    }
    
    return img;

}
