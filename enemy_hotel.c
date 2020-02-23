#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "globals.h"
#include "linked_list.h"
#include "bullet.h"
#include "enemy.h"
#include "enemy_hotel.h"

extern SDL_Renderer *renderer;

extern Mix_Chunk *soundEnemyShoot;
extern Mix_Chunk *soundEnemyHit;
extern Mix_Chunk *soundEnemyDead;

extern SDL_Texture *imgEnemyHotel_L1_00;
extern SDL_Texture *imgEnemyHotel_L1_01;
extern SDL_Texture *imgEnemyHotel_L2_00;
extern SDL_Texture *imgEnemyHotel_L2_01;


extern struct Node *listBullet;





void init_enemy_hotel(struct Enemy *enemy) {
	struct EnemyHotel *enemyhotel = malloc(sizeof(struct EnemyHotel));
	
	switch(enemy->iLevel) {
		case 1:
			enemy->iHealth = 3;
			enemyhotel->fMaxShootDelay = 1;
			enemyhotel->fShootSpeed = 1;
			break;
		case 2:
			enemy->iHealth = 10;
			enemyhotel->fMaxShootDelay = 0.5;
			enemyhotel->fShootSpeed = 2;
			break;
			
	}

	enemyhotel->fShootDelay = enemyhotel->fMaxShootDelay;
	enemyhotel->fShootAngle = 0;
	enemy->subtype = enemyhotel;
}

void update_enemy_hotel(struct Enemy *enemy) {
	struct EnemyHotel *enemyhotel = (struct EnemyHotel *) enemy->subtype;
	
	enemyhotel->fShootDelay -= DELTA_TIME;
	if (enemyhotel->fShootDelay <= 0) {
		shoot_enemy_hotel(enemy);
		enemyhotel->fShootDelay += enemyhotel->fMaxShootDelay;
		enemyhotel->fShootAngle += 15;
		if (enemyhotel->fShootAngle > 360) {
			enemyhotel->fShootAngle -= 360;
		}
	}

}

void shoot_enemy_hotel(struct Enemy *enemy) {
	struct Bullet *bullet = malloc(sizeof(struct Bullet));
	struct EnemyHotel *enemyhotel = (struct EnemyHotel *) enemy->subtype;

	init_bullet(bullet, enemy->x + enemy->width / 2, enemy->y + enemy->height / 2, 0);

	
	bullet->vel_x = enemyhotel->fShootSpeed * cos(enemyhotel->fShootAngle * PI / 180);
	bullet->vel_y = enemyhotel->fShootSpeed * sin(enemyhotel->fShootAngle * PI / 180);
	bullet->iHitsPlayer = TRUE;
	
	add_node(&listBullet, bullet);
	Mix_PlayChannel(-1, soundEnemyShoot, 0);

	
	
}

SDL_Texture *getTexture_enemy_hotel(struct Enemy *enemy) {

	SDL_Texture *img;
	
	int iSpriteIndex = ((int) (enemy->fLifetime * 2)) % 2; //change sprite every 0.5 seconds
//	SDL_Rect rect = { enemy->x, enemy->y, enemy->width, enemy->height };
	
	
//	printf("draw_enemy_hotel\n");
//	rect.x = enemy->x;
//	rect.y = enemy->y;
//	rect.w = enemy->width;
//	rect.h = enemy->height;
	
	if (enemy->fDamagedCountdown > 0) {
//		SDL_SetTextureColorMod(img, 255, 0, 0);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	} else {
//		SDL_SetTextureColorMod(img, 255, 255, 255);
		SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
	}
	
	if (iSpriteIndex == 0) {
		if (enemy->iLevel == 1) {
			img = imgEnemyHotel_L1_00;
		} else if (enemy->iLevel == 2) {
			img = imgEnemyHotel_L2_00;
		}
    } else if (iSpriteIndex == 1) {
	    if (enemy->iLevel == 1) {
			img = imgEnemyHotel_L1_01;
		} else if (enemy->iLevel == 2) {
			img = imgEnemyHotel_L2_01;

		}
	}


	
	
//	SDL_RenderFillRect(renderer, &rect);
	return img;

}

void damage_enemy_hotel(struct Enemy *enemy, int iDamageAmount) {
	enemy->iHealth -= iDamageAmount;
	if (enemy->iHealth <= 0) {
		kill_enemy(enemy);
	} else {
		Mix_PlayChannel(-1, soundEnemyHit, 0);

		
	}
	
	enemy->fDamagedCountdown = 0.2;


}
