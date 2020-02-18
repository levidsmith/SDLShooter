//2020 Levi D. Smith
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "globals.h"
#include "enemy.h"
#include "enemy_kilo.h"
#include "screen_game.h"

extern SDL_Surface *surfaceEnemyKilo_L1_00;
extern SDL_Surface *surfaceEnemyKilo_L1_01;
extern SDL_Surface *surfaceEnemyKilo_L2_00;
extern SDL_Surface *surfaceEnemyKilo_L2_01;

extern SDL_Surface *generateSurface(char *strFile);

//SDL_Texture *imgNumberValue;
//SDL_Texture *imgNumberZero;
//SDL_Texture *imgNumberOne;

extern TTF_Font *fontDefault;
extern SDL_Renderer *renderer;

void init_enemy_kilo(struct Enemy *enemy) {
	printf("init_enemy_kilo\n");
	
	int iBodyParts;
//	iBodyParts = 5;
	iBodyParts = (rand() % 3) + 3;


    
    struct EnemyKiloHead *enemykilohead = malloc(sizeof(struct EnemyKiloHead));
//    int iTargetValue = (rand() % 14) + 1;
    int iTargetValue = (rand() % (int) (pow(2, iBodyParts) - 1)) + 1;
    enemykilohead->iTargetValue = iTargetValue;
    enemy->subtype = enemykilohead;

    char strText[8];
    sprintf(strText, "%d", iTargetValue);
    SDL_Color colorRedText = {255, 0, 0};
//    generateTextTexture(&imgText, strText, colorRedText, fontDefault);

	SDL_Surface *sprText;
//	sprText = TTF_RenderText_Solid(font, strText, colorText);

/*
	if (*imgText != NULL) {
	  SDL_DestroyTexture(*imgText);
	}
	*imgText = SDL_CreateTextureFromSurface(renderer, sprText);
    SDL_FreeSurface(sprText); 
*/

	SDL_Surface *surfaceHead;
	SDL_Surface *surfaceNumber;
	SDL_Rect rect = {32, 32, 64, 64};
	SDL_Texture *imgHead;

	surfaceNumber = TTF_RenderText_Solid(fontDefault, strText, colorRedText);

    surfaceHead = generateSurface("assets/images/enemy_kilo_l1_1.bmp");
	rect.x = (64 - surfaceNumber->w) / 2;
	rect.y = (64 - surfaceNumber->h) / 2;
	SDL_BlitSurface(surfaceNumber, NULL, surfaceHead, &rect);
	imgHead = SDL_CreateTextureFromSurface(renderer, surfaceHead);
	SDL_FreeSurface(surfaceHead);
    enemykilohead->imgEnemyKilo_L1_00 = imgHead;

    surfaceHead = generateSurface("assets/images/enemy_kilo_l1_2.bmp");
	SDL_BlitSurface(surfaceNumber, NULL, surfaceHead, &rect);
	imgHead = SDL_CreateTextureFromSurface(renderer, surfaceHead);
	SDL_FreeSurface(surfaceHead);
    enemykilohead->imgEnemyKilo_L1_01 = imgHead;




    
    struct Enemy *enemyPart;
    struct EnemyKiloBody *enemykilobody;




    int iWidth = 64;
    int iHeight = 64;
	int x_pos = enemy->x + (((iBodyParts - 1) * iWidth) / 2);
	int y_pos = enemy->y + 64;
	
	int i;
	i = 0;
	struct Enemy *enemyPrevious = NULL;
	while (i < iBodyParts) {

		//new body part
		enemyPart = malloc(sizeof(struct Enemy));
//		init_enemy(enemyPart, enemy->x - (iWidth * 1.5), enemy->y + 64, 10, 2, FALSE);
		init_enemy(enemyPart, x_pos, y_pos, 10, 2, FALSE);
		enemykilobody = malloc(sizeof(struct EnemyKiloBody));
		enemykilobody->head = enemy;
		enemykilobody->iValue = 0;
		enemykilobody->next = NULL;
		enemyPart->subtype = enemykilobody;
		enemyPart->width = iWidth;
		enemyPart->height = iHeight;
		
		reloadBodyTexture(enemykilobody);


		
		if (i == 0) {
			enemykilohead->bodyFirst = enemyPart;
		} else {
			((struct EnemyKiloBody *) enemyPrevious->subtype)->next = enemyPart;
		}
		
		enemyPrevious = enemyPart;
		x_pos -= 64;

		
		i++;
	}
/*    
    //first part
    enemyPart = malloc(sizeof(struct Enemy));
    init_enemy(enemyPart, enemy->x - (iWidth * 1.5), enemy->y + 64, 10, 2, FALSE);
    enemykilobody = malloc(sizeof(struct EnemyKiloBody));
    enemykilobody->head = enemy;
	enemykilobody->iValue = 0;
    enemyPart->subtype = enemykilobody;
    enemyPart->width = iWidth;
    enemyPart->height = iHeight;
    enemykilohead->body[0] = enemyPart;

    //second part
    enemyPart = malloc(sizeof(struct Enemy));
    init_enemy(enemyPart, enemy->x - (iWidth * 0.5), enemy->y + 64, 10, 2, FALSE);
    enemykilobody = malloc(sizeof(struct EnemyKiloBody));
    enemykilobody->head = enemy;
	enemykilobody->iValue = 0;
    enemyPart->subtype = enemykilobody;
    enemyPart->width = iWidth;
    enemyPart->height = iHeight;
    enemykilohead->body[1] = enemyPart;

    //third part
    enemyPart = malloc(sizeof(struct Enemy));
    init_enemy(enemyPart, enemy->x + (iWidth * 0.5), enemy->y + 64, 10, 2, FALSE);
    enemykilobody = malloc(sizeof(struct EnemyKiloBody));
    enemykilobody->head = enemy;
	enemykilobody->iValue = 0;
    enemyPart->subtype = enemykilobody;
    enemyPart->width = iWidth;
    enemyPart->height = iHeight;
    enemykilohead->body[2] = enemyPart;

    //fourth part
    enemyPart = malloc(sizeof(struct Enemy));
    init_enemy(enemyPart, enemy->x + (iWidth * 1.5), enemy->y + 64, 10, 2, FALSE);
    enemykilobody = malloc(sizeof(struct EnemyKiloBody));
    enemykilobody->head = enemy;
	enemykilobody->iValue = 0;
    enemyPart->subtype = enemykilobody;
    enemyPart->width = iWidth;
    enemyPart->height = iHeight;
    enemykilohead->body[3] = enemyPart;
	*/
}


void reloadBodyTexture(struct EnemyKiloBody *enemykilobody) {
    char strText[8];


	//start body images
    sprintf(strText, "%d", enemykilobody->iValue);
    SDL_Color colorGreenText = {0, 128, 0};


	SDL_Surface *surfaceBody;
	SDL_Texture *imgBody;
	SDL_Surface *surfaceNumber;
	SDL_Rect rect = {32, 32, 64, 64};

	surfaceNumber = TTF_RenderText_Solid(fontDefault, strText, colorGreenText);

    surfaceBody = generateSurface("assets/images/enemy_kilo_l2_1.bmp");
	rect.x = (64 - surfaceNumber->w) / 2;
	rect.y = (64 - surfaceNumber->h) / 2;
	SDL_BlitSurface(surfaceNumber, NULL, surfaceBody, &rect);
	imgBody = SDL_CreateTextureFromSurface(renderer, surfaceBody);
	SDL_FreeSurface(surfaceBody);
    enemykilobody->imgEnemyKilo_L2_00 = imgBody;

    surfaceBody = generateSurface("assets/images/enemy_kilo_l2_2.bmp");
	SDL_BlitSurface(surfaceNumber, NULL, surfaceBody, &rect);
	imgBody = SDL_CreateTextureFromSurface(renderer, surfaceBody);
	SDL_FreeSurface(surfaceBody);
    enemykilobody->imgEnemyKilo_L2_01 = imgBody;

//end body images

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
		reloadBodyTexture(enemykilobody);
        check_destroy_enemy_kilo(enemykilobody->head);
		
    }
    
}

void check_destroy_enemy_kilo(struct Enemy *enemy) {
    int iAttackValue = 0;
    
    struct EnemyKiloHead *enemykilohead = (struct EnemyKiloHead *) enemy->subtype;
	
	struct Enemy *enemyPart = (struct Enemy *) enemykilohead->bodyFirst;
	int iBitValue = 1;
	while (enemyPart != NULL) {
		struct EnemyKiloBody *enemykilobody = (struct EnemyKiloBody *) enemyPart->subtype;
		printf("%d\n", enemykilobody->iValue);
		iAttackValue += (iBitValue * enemykilobody->iValue);
		iBitValue *= 2;
		enemyPart = enemykilobody->next;
	}
	printf("finished loop\n");
	
	
	/*
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
*/

    if (iAttackValue == enemykilohead->iTargetValue) {
		enemyPart = (struct Enemy *) enemykilohead->bodyFirst;
		while (enemyPart != NULL) {
			struct EnemyKiloBody *enemykilobody = (struct EnemyKiloBody *) enemyPart->subtype;
			destroy_enemy(enemyPart);
			enemyPart = enemykilobody->next;
		}
		
		destroy_enemy(enemy);


	}

/*	
    
    if (iAttackValue == enemykilohead->iTargetValue) {
        destroy_enemy(enemyPart1);
        destroy_enemy(enemyPart2);
        destroy_enemy(enemyPart3);
        destroy_enemy(enemyPart4);
        destroy_enemy(enemy);

    }
*/    
    
    
}

SDL_Texture *getTexture_enemy_kilo(struct Enemy *enemy) {
    SDL_Texture *img = NULL;
    
    
       int iSpriteIndex = ((int) (enemy->fLifetime * 2)) % 2; //change sprite every 0.5 seconds
       
/*       
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
*/	 
    
    if (enemy->iLevel == 1) {
        struct EnemyKiloHead *enemykilohead = (struct EnemyKiloHead *) enemy->subtype;
		if (iSpriteIndex == 0) {
			img = enemykilohead->imgEnemyKilo_L1_00;
        } else if (iSpriteIndex == 1) {
			img = enemykilohead->imgEnemyKilo_L1_01;
		}
			
    } else {
        struct EnemyKiloBody *enemykilobody = (struct EnemyKiloBody *) enemy->subtype;
		if (iSpriteIndex == 0) {
			img = enemykilobody->imgEnemyKilo_L2_00;
        } else if (iSpriteIndex == 1) {
			img = enemykilobody->imgEnemyKilo_L2_01;
		}
    }
    
    return img;

}
