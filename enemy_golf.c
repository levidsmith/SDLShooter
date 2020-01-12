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




void init_enemy_golf(struct Enemy *enemy) {
	enemy->subtype = malloc(sizeof(struct EnemyGolf));
	struct EnemyGolf *enemygolfhead = (struct EnemyGolf *) enemy->subtype;
	enemygolfhead->previous = NULL;
	enemygolfhead->iIsHead = TRUE;
	
	enemygolfhead->iFoo = 42;
	printf("init_enemy_golf: %d\n", enemygolfhead->iFoo);
	int iVel = rand() % 2;
	if (iVel == 0) {
		enemy->vel_x = 2;
	} else {
		enemy->vel_x = -2;
		
	}
	
	int iSegments = 3 + (rand() % 3);
//	int iSegments = 1;
	
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
		init_enemy_golf_tail(enemyTail1, enemy);

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
	
	/*
	struct Enemy *enemyTail1 = malloc(sizeof(struct Enemy));
	init_enemy(enemyTail1, enemy->x - UNIT_SIZE, enemy->y, enemy->iType, enemy->iLevel, FALSE);
	init_enemy_golf_tail(enemyTail1, enemy);
	struct EnemyGolf *enemygolftail1 = malloc(sizeof(struct EnemyGolf));
	enemygolftail1->iFoo = 43;
	enemyTail1->subtype = enemygolftail1;
	enemygolfhead->next = enemyTail1;
	enemygolftail1->previous = enemy;
	enemygolftail1->iIsHead = FALSE;



	struct Enemy *enemyTail2 = malloc(sizeof(struct Enemy));
	init_enemy(enemyTail2, enemyTail1->x - UNIT_SIZE, enemyTail1->y, enemy->iType, enemy->iLevel, FALSE);
	init_enemy_golf_tail(enemyTail2, enemy);
	struct EnemyGolf *enemygolftail2 = malloc(sizeof(struct EnemyGolf));
	enemygolftail2->iFoo = 44;
	enemyTail2->subtype = enemygolftail2;
	enemygolftail1->next = enemyTail2;
	enemygolftail2->previous = enemyTail1;
	enemygolftail2->iIsHead = FALSE;
*/





}

void init_enemy_golf_tail(struct Enemy *enemy, struct Enemy *previous) {
	/*
	enemy->subtype = malloc(sizeof(struct EnemyGolf));
	struct EnemyGolf *enemygolf = (struct EnemyGolf *) enemy->subtype;
	
	((struct EnemyGolf *) previous->subtype)->next = enemy;
	enemygolf->next = NULL;
	*/

}
 

void update_enemy_golf(struct Enemy *enemy) {
//	printf("start update_enemy_golf\n");
	
	if (enemy != NULL && enemy->subtype != NULL) {
		struct EnemyGolf *enemygolf = (struct EnemyGolf *) enemy->subtype;
//		printf("a update_enemy_golf id: %d\n", enemygolf->iFoo);
		
		if (enemygolf->iIsHead) {
			updateMinMaxValues(enemy);
			printf("x_min: %f, x_max: %f, y_min: %f, y_max: %f\n", enemygolf->x_min, enemygolf->x_max, enemygolf->y_min, enemygolf->y_max);

			if (enemygolf->x_min > SCREEN_WIDTH) {
				moveAll(enemy, -enemygolf->x_max, 0);
			} else if (enemygolf->x_max < 0) {
				moveAll(enemy, SCREEN_WIDTH - enemygolf->x_min, 0);
			}
	
			
		}
		
		
	
	
	/*
		if (enemy->x > SCREEN_WIDTH) {
			enemy->x = 0 - enemy->width;
		}
		*/
//		printf("b update_enemy_golf id: %d\n", enemygolf->iFoo);
	
		if (enemygolf->previous != NULL) {
			//enemy->x = enemygolf->previous->x - UNIT_SIZE;
			float x_diff = enemygolf->previous->x - enemy->x;
			float y_diff = enemygolf->previous->y - enemy->y;
			float fMag = sqrt(pow(x_diff, 2) + pow(y_diff, 2));
			float fAngleX = acos(x_diff / fMag);
			float fAngleY = asin(y_diff / fMag);
//			printf("current x %f, y %f, previous x %f, y %f\n", enemy->x, enemy->y, enemygolf->previous->x, enemygolf->previous->y);
			printf("x_diff: %f, y_diff: %f, AngleX: %f, AngleY: %f, fMag: %f\n", x_diff, y_diff, fAngleX, fAngleY, fMag);
			
			if (fMag > 1 * UNIT_SIZE) {
				enemy->x += cos(fAngleX) * (fMag - UNIT_SIZE);
				enemy->y += sin(fAngleY) * (fMag - UNIT_SIZE);
			}
			
			
			
		} else {
			enemy->x += enemy->vel_x * UNIT_SIZE * DELTA_TIME;
			//enemy->y = enemy->orig_y + (UNIT_SIZE * sin(enemy->fActiveTime * PI));
			enemy->y = enemy->orig_y + (UNIT_SIZE * sin(enemy->fLifetime * PI * 0.5));

			
		}
	}

	
//	printf("end update_enemy_golf\n");
	
}


void draw_enemy_golf(struct Enemy *enemy) {
	
	if (enemy != NULL) {
		struct EnemyGolf *enemygolf = (struct EnemyGolf *) enemy->subtype;
//		printf("draw_enemy_golf id: %d\n", enemygolf->iFoo);
		
		SDL_Rect rect = { enemy->x, enemy->y, enemy->width, enemy->height };
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
	
	}

/*
	struct Enemy *enemyTail1 = ((struct EnemyGolf *) enemy->subtype)->next;
	struct EnemyGolf *enemygolftail1 = (struct EnemyGolf *) enemyTail1->subtype;
	printf("enemyTail1 value: %d\n", enemygolftail1->iFoo);

	if (enemyTail1 != NULL) {
		if (enemyTail1->isAlive) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		} else {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			
		}
	rect.x = enemyTail1->x;
	rect.y = enemyTail1->y;
	rect.w = enemyTail1->width;
	rect.h = enemyTail1->height;
	SDL_RenderFillRect(renderer, &rect);
	}
	

	*/
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
				destroy_enemy(enemy);
				//delete the head and all tail segments
				struct Enemy *current = ((struct EnemyGolf * ) enemy->subtype)->next;
				while (current != NULL) {
					
					destroy_enemy(current);

					current = ((struct EnemyGolf * )current->subtype)->next;
				}

				Mix_PlayChannel(-1, soundEnemyDead, 0);
				
			} else {
				Mix_PlayChannel(-1, soundEnemyHit, 0);

			}
			

			
			
			
		} else {
			Mix_PlayChannel(-1, soundEnemyShield, 0);

		}

	} else {
		if (enemy->iHealth > 0) {
			enemy->iHealth -= iDamageAmount;
			Mix_PlayChannel(-1, soundEnemyHit, 0);
		} else {
			//segment is already destroyed
			Mix_PlayChannel(-1, soundEnemyShield, 0);
			
		}

		
	}
	
}

int getIsTailDead(struct Enemy *enemy) {
	int iIsTailDead = TRUE;
//	printf("start getIsTailDead\n");
//			struct EnemyGolf *current = (struct EnemyGolf * ) enemygolf->next->subtype;


	if (enemy != NULL && enemy->subtype != NULL) {
//		printf("a getIsTailDead\n");
			struct Enemy *current = ((struct EnemyGolf * ) enemy->subtype)->next;
//		printf("b getIsTailDead\n");
			while (current != NULL) {
//		printf("c getIsTailDead\n");
				if (current->iHealth > 0) {
		//printf("c1 getIsTailDead\n");
					iIsTailDead = FALSE;
				}

//		printf("c2 getIsTailDead\n");

				if (current->subtype != NULL) {
					current = ((struct EnemyGolf * )current->subtype)->next;
				} else {
					current = NULL;
				}
//		printf("d getIsTailDead\n");
			}
//		printf("e getIsTailDead\n");
			
	}
//	printf("end getIsTailDead\n");
			return iIsTailDead;
	
}

void updateMinMaxValues(struct Enemy *enemy) {
	struct EnemyGolf *enemygolfhead;
	
	if (enemy != NULL && enemy->subtype != NULL) {
			enemygolfhead = (struct EnemyGolf * ) enemy->subtype;
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
					current = ((struct EnemyGolf * )current->subtype)->next;
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

			enemygolf = (struct EnemyGolf * ) enemy->subtype;

			
			struct Enemy *current = enemygolf->next;

			while (current != NULL) {
				enemygolf = (struct EnemyGolf *) current->subtype;
				
				current->x += x_move;
				current->y += y_move;


				if (current->subtype != NULL) {
					current = ((struct EnemyGolf * )current->subtype)->next;
				} else {
					current = NULL;
				}
			}
			
	}	
}
