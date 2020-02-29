//2020 Levi D. Smith

#include "globals.h"
#include "linked_list.h"
#include "util.h"
#include "enemy.h"
#include "enemy_mike.h"
#include "ship.h"
#include "bullet.h"

extern SDL_Texture *imgEnemyMike_L1_00;
extern SDL_Texture *imgEnemyMike_L1_01;
extern SDL_Texture *imgEnemyMike_L1_02;
extern SDL_Texture *imgEnemyMike_L1_03;
extern SDL_Renderer *renderer;

extern struct Ship *ship;
extern struct Node *listBullet;

extern Mix_Chunk *soundEnemyShoot;




void init_enemy_mike(struct Enemy *enemy) {
	
	if (enemy->isRoot) {
		struct EnemyMikeHead *enemymikehead = malloc(sizeof(struct EnemyMikeHead));
		enemymikehead->fRot = 0;
		enemymikehead->fMinRotSpeed = 0.2;
		enemymikehead->fRotSpeed = enemymikehead->fMinRotSpeed;
		enemymikehead->fErrorRotSpeed = 4;
		enemymikehead->fSpeed = getRandomFloat(1, 2);
//		enemymikehead->fSpeed = 0;

		enemymikehead->iShootTimes = 0;
		enemymikehead->fShootDelay = 0;
		enemymikehead->fMaxShootDelay = 0.5;
		

		
		enemymikehead->iVulnerable[0] = FALSE;
		enemymikehead->iVulnerable[1] = FALSE;
		enemymikehead->iVulnerable[2] = FALSE;
		
		if (enemy->iLevel == 1) {
			int iRand = getRandomInt(0, 3);
			switch(iRand) {
				case 0:
					enemymikehead->iVulnerable[0] = TRUE;
					break;
				case 1:
					enemymikehead->iVulnerable[1] = TRUE;
					break;
				case 2:	
					enemymikehead->iVulnerable[2] = TRUE;
					break;
			}
		} else if (enemy->iLevel == 2) {
			//int iRand = getRandomInt(0, 4);
			int iRand = getRandomInt(0, 3);
			switch(iRand) {
				case 0:
					enemymikehead->iVulnerable[0] = TRUE;
					enemymikehead->iVulnerable[1] = TRUE;
					break;
				case 1:
					enemymikehead->iVulnerable[0] = TRUE;
					enemymikehead->iVulnerable[2] = TRUE;
					break;
				case 2:	
					enemymikehead->iVulnerable[1] = TRUE;
					enemymikehead->iVulnerable[2] = TRUE;
					break;
					/*
				case 3:	
					enemymikehead->iVulnerable[0] = TRUE;
					enemymikehead->iVulnerable[1] = TRUE;
					enemymikehead->iVulnerable[2] = TRUE;
					break;
					*/
			}
			
		}
			  
		enemy->subtype = enemymikehead;
		
		
		struct EnemyMikeBody *enemymikebody;
		struct Enemy *enemybody;
		
        enemybody = malloc(sizeof(struct Enemy));
//        init_enemy(enemybody, enemy->x + (UNIT_SIZE * 1), enemy->y, 12, 2, FALSE);
        init_enemy(enemybody, getCenterX_enemy(enemy), getCenterY_enemy(enemy), 12, 2, FALSE);
		enemymikebody = malloc(sizeof(struct EnemyMikeBody));
		enemymikebody->imgTexture = imgEnemyMike_L1_01;
		enemymikebody->iValue = 0;
		enemymikebody->iIndex = 0;
		enemymikebody->head = enemy;
		enemybody->width = 32;
		enemybody->height = 32;
		enemybody->subtype = enemymikebody;
		enemymikehead->body[0] = enemybody;


        enemybody = malloc(sizeof(struct Enemy));
//        init_enemy(enemybody, enemy->x + (UNIT_SIZE * 2), enemy->y, 12, 2, FALSE);
        init_enemy(enemybody, getCenterX_enemy(enemy), getCenterY_enemy(enemy), 12, 2, FALSE);
		enemymikebody = malloc(sizeof(struct EnemyMikeBody));
		enemymikebody->imgTexture = imgEnemyMike_L1_02;
		enemymikebody->iValue = 1;
		enemymikebody->iIndex = 1;
		enemymikebody->head = enemy;
		enemybody->width = 32;
		enemybody->height = 32;
		enemybody->subtype = enemymikebody;
		enemymikehead->body[1] = enemybody;

        enemybody = malloc(sizeof(struct Enemy));
//        init_enemy(enemybody, enemy->x + (UNIT_SIZE * 3), enemy->y, 12, 2, FALSE);
        init_enemy(enemybody, getCenterX_enemy(enemy), getCenterY_enemy(enemy), 12, 2, FALSE);
		enemymikebody = malloc(sizeof(struct EnemyMikeBody));
		enemymikebody->imgTexture = imgEnemyMike_L1_03;
		enemymikebody->iValue = 2;
		enemymikebody->iIndex = 2;
		enemymikebody->head = enemy;
		enemybody->width = 32;
		enemybody->height = 32;
		enemybody->subtype = enemymikebody;
		enemymikehead->body[2] = enemybody;


		
	}
}

void update_enemy_mike(struct Enemy *enemy) {
	if (enemy->isRoot) {
		struct EnemyMikeHead *enemymikehead = (struct EnemyMikeHead *) enemy->subtype;
		enemymikehead->fRot += enemymikehead->fRotSpeed * 2 * M_PI * DELTA_TIME;
		if (enemymikehead->fRot > 2 * M_PI) {
			enemymikehead->fRot -= 2 * M_PI;
		}
		
		if (enemymikehead->fRotSpeed > enemymikehead->fMinRotSpeed) {
			enemymikehead->fRotSpeed -= 0.5 * DELTA_TIME;
			
			if (enemymikehead->fRotSpeed < enemymikehead->fMinRotSpeed) {
				enemymikehead->fRotSpeed = enemymikehead->fMinRotSpeed;
			}
			
		}
		
		enemy->x += DELTA_TIME * enemymikehead->fSpeed * UNIT_SIZE;
		if (enemy->x > SCREEN_WIDTH) {
			enemy->x -= SCREEN_WIDTH;
		}
		enemy->y = enemy->orig_y + (1 * UNIT_SIZE) * sin(enemy->fLifetime * M_PI * 0.5);
		
		//shoot if in attack mode
		if (enemymikehead->iShootTimes > 0) {
			printf("shoot times: %d\n", enemymikehead->iShootTimes);
			enemymikehead->fShootDelay -= DELTA_TIME;
			if (enemymikehead->fShootDelay <= 0) {
				shoot_enemy_mike(enemy);
				enemymikehead->iShootTimes--;
				enemymikehead->fShootDelay += enemymikehead->fMaxShootDelay;
			}
		}
		
		
		int i;
		for (i = 0; i < 3; i++) {
			if (enemymikehead->body[i] != NULL) {
				update_enemy_mike_body(enemymikehead->body[i]);
			}
		}
		
		
		
	}
}


void update_enemy_mike_body(struct Enemy *enemy) {
		struct EnemyMikeBody *enemymikebody = (struct EnemyMikeBody *) enemy->subtype;
		struct Enemy *enemyhead = enemymikebody->head;
		struct EnemyMikeHead *enemymikehead = (struct EnemyMikeHead *) enemyhead->subtype;
		float fBodyRot = enemymikehead->fRot + (enemymikebody->iValue * 2 * M_PI / 3);
		enemy->x = getCenterX_enemy(enemyhead) + ((enemyhead->width + enemy->width) / 2) * cos(fBodyRot) - (enemy->width / 2);
		enemy->y = getCenterY_enemy(enemyhead) + ((enemyhead->height + enemy->height) / 2) * sin(fBodyRot) - (enemy->height / 2);
	
}


SDL_Texture *getTexture_enemy_mike(struct Enemy *enemy) {
	SDL_Texture *imgTexture;
	imgTexture = NULL;
	
	if (enemy->isRoot) {
		struct EnemyMikeHead *enemymikehead = (struct EnemyMikeHead *) enemy->subtype;
		imgTexture = imgEnemyMike_L1_00;
			
		SDL_SetTextureColorMod(imgTexture, enemymikehead->iVulnerable[0] * 255, enemymikehead->iVulnerable[1] * 255, enemymikehead->iVulnerable[2] * 255);
		
	} else {
		struct EnemyMikeBody *enemymikebody = (struct EnemyMikeBody *) enemy->subtype;
		imgTexture = enemymikebody->imgTexture;
		switch(enemymikebody->iValue) {
			case 0:
				SDL_SetTextureColorMod(imgTexture, 255, 0, 0);
				break;
			case 1:
				SDL_SetTextureColorMod(imgTexture, 0, 255, 0);
				break;
			case 2:
				SDL_SetTextureColorMod(imgTexture, 0, 0, 255);
				break;
		}
		
		/*
		if (enemy->iHealth <= 0) {
			SDL_SetTextureColorMod(imgTexture, 0, 0, 0);
			
		}
		*/
	}
	
	return imgTexture;
}

void destroy_enemy_mike(struct Enemy *enemy) {
}

void damage_enemy_mike(struct Enemy *enemy, int iDamageAmount) {
	if (!enemy->isRoot) {
		struct EnemyMikeBody *enemymikebody = (struct EnemyMikeBody *) enemy->subtype;
		struct Enemy *enemyhead = (struct Enemy *) enemymikebody->head;
		struct EnemyMikeHead *enemymikehead = (struct EnemyMikeHead *) enemyhead->subtype;
		
		printf("body hit: %d\n", enemymikebody->iValue);
		printf("head values: %d, %d, %d\n", enemymikehead->iVulnerable[0], enemymikehead->iVulnerable[1], enemymikehead->iVulnerable[2]);
		
		if (enemymikehead->iVulnerable[enemymikebody->iValue] == TRUE) {
			enemy->iHealth -= iDamageAmount;
			
			if (enemy->iHealth <= 0) {
				kill_enemy(enemy);
//				enemy->isAlive = FALSE;
				enemymikehead->body[enemymikebody->iIndex] = NULL;
				check_head_kill_enemy_mike(enemyhead);
			}
		} else {
			enemymikehead->fRotSpeed = enemymikehead->fErrorRotSpeed;
				attack_mode_enemy_mike(enemyhead);

		}
	}
	
}

void attack_mode_enemy_mike(struct Enemy *enemy) {
	struct EnemyMikeHead *enemymikehead = (struct EnemyMikeHead *) enemy->subtype;
	enemymikehead->fShootDelay = 0;
	enemymikehead->iShootTimes = 3;
	
}

void shoot_enemy_mike(struct Enemy *enemy) {
	struct EnemyMikeHead *enemymikehead = (struct EnemyMikeHead *) enemy->subtype;

	struct Bullet *bullet;
	
	  if (enemy != NULL && (enemy->fShootDelay <= 0) && enemy->isAlive) {


		bullet 	= malloc(sizeof(struct Bullet));

		init_bullet(bullet, enemy->x + enemy->width / 2, enemy->y + enemy->height / 2, 0);

		float fDistance	= getDistance(ship->x, ship->y, bullet->x, bullet->y);
		bullet->vel_x = 5 * (getCenterX_ship(ship) - getCenterX_bullet(bullet)) / fDistance;
		bullet->vel_y = 5 * (getCenterY_ship(ship) - getCenterY_bullet(bullet)) / fDistance;
		bullet->iHitsPlayer = TRUE;
	
		add_node(&listBullet, bullet);
		Mix_PlayChannel(-1, soundEnemyShoot, 0);
	  }
	
	
}

void check_head_kill_enemy_mike(struct Enemy *enemy) {
	struct EnemyMikeHead *enemymikehead = (struct EnemyMikeHead *) enemy->subtype;
	int isKilled = TRUE;
	int i;
	
	for (i = 0; i < 3; i++) {
		if (enemymikehead->body[i] == NULL) {
			printf("body %d is NULL\n", i);
		} else if (!enemymikehead->iVulnerable[i]) {
			printf("body %d vulnerable FALSE\n", i);
		} else if (enemymikehead->iVulnerable[i]) {
			struct Enemy *enemybody = enemymikehead->body[i];
			struct EnemyMikeBody *enemymikebody = (struct EnemyMikeBody *) enemybody->subtype;
			printf("body %d isAlive: %d\n", i, enemybody->isAlive);
//			if (enemybody->isAlive) {
			if (enemybody->iHealth > 0) {
				isKilled = FALSE;
				
			}
		}
	}
	
	if (isKilled) {
		for (i = 0; i < 3; i++) {
			if (enemymikehead->body[i] != NULL) {
				struct Enemy *enemybody = (struct Enemy *) enemymikehead->body[i];
				if (enemybody->isAlive) {
					kill_enemy(enemybody);
				}
			}
		}
		kill_enemy(enemy);
		
	}
		
	
}