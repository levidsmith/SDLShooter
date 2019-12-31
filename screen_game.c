//2019 Levi D. Smith
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "screen_game.h"
#include "linked_list.h"
#include "enemy.h"
#include "ship.h"
#include "level_reader.h"
#include "bullet.h"
#include "powerup.h"
#include "explosion.h"

#define BACKGROUND_ROWS 4
#define BACKGROUND_COLS 5



//FUNCTION PROTOTYPES
extern struct Node *add_node(struct Node **head, void *value);
extern int count_list(struct Node *head);
extern void clear_list(struct Node **head);
extern void remove_node(struct Node **head, struct Node *node);
extern SDL_Renderer* renderer;
extern void setCurrentScreen(int iScreen);
extern void damage_enemy(struct Enemy *enemy, int iDamageAmount);
extern SDL_Texture *imgFireButton[16];
extern SDL_Texture *imgFireButtonText[3];


//VARIABLES
extern TTF_Font *fontDefault;
extern TTF_Font *fontLarge;


extern Mix_Music *musicGame; 
extern SDL_Texture *imgBackground[2];
extern SDL_Texture *imgScoreText;
extern SDL_Texture *imgLevelCompleteText;
extern SDL_Texture *imgGameOverText;
extern SDL_Texture *imgLevel;
extern SDL_Texture *imgWeaponText;
extern SDL_Texture *imgGameTimeText;


extern Mix_Chunk *soundShoot;
extern Mix_Chunk *soundShipDead;
extern Mix_Chunk *soundEnemyDead;
extern Mix_Chunk *soundEnemyShoot;
extern Mix_Chunk *soundEnemyHit;
extern Mix_Chunk *soundPowerup;


SDL_Rect pos;
SDL_Rect posText;

int iButtonFire1Down = FALSE;
int iButtonFire2Down = FALSE;
int iButtonFire3Down = FALSE;
int iButtonOptionDown = FALSE;

struct Ship *ship;
struct Node *listBullet;
struct Node *listEnemy;
struct Node *listPowerup;
struct Node *listExplosion;


extern int iKeepLooping;
extern int iGameContinue;
extern int iCanContinue;
extern int iTitleMenuChoice;
int iBackgroundOffset;
int iLevelComplete = FALSE;
int iGameOver = FALSE;

int iScore;
int iCurrentLevel = 0;
int iCurrentWorld = 1;
int iLevelCount = -1;

float fKeyPressDelay = 0;


char *strWeaponNames[7] = {"Normal", "Speed Shot", "Multi Shot", "Wave Shot", "Blast Shot", "Spin Shot" };
int iBackgroundPattern[BACKGROUND_ROWS][BACKGROUND_COLS];

time_t timeStartGame;
time_t timeEndGame;



void start_screen_game() {
  printf("start\n");

  clear_list(&listBullet);
  clear_list(&listEnemy);
  clear_list(&listPowerup);
  clear_list(&listExplosion);

	
  ship = malloc(sizeof(struct Ship));
  init_ship(ship);
  
  printf("ship values x: %d y: %d width: %d height %d\n", ship->x, ship->y, ship->width, ship->height);

  loadWorld();
  
  iScore = 0;
  updateScoreText();
  updateBackgroundPattern(4);
  

  iLevelComplete = FALSE;
  iGameOver = FALSE;



    //initialize game time
    if (&timeStartGame == NULL || !iGameContinue) {
        printf("Initialize time\n");
        time(&timeStartGame);
    }
    
    //enable continue option on title screen
    iCanContinue = TRUE;
    
  Mix_VolumeMusic(MIX_MAX_VOLUME * 0.2);
  Mix_PlayMusic(musicGame, -1);

}

void loadWorld() {
	char strLevelFile[32];
	
	sprintf(strLevelFile, "%s0%d%s", LEVEL_FILE_PREFIX, iCurrentWorld, LEVEL_FILE_SUFFIX);
	
  //read level
    if (!iGameContinue) {
        iCurrentLevel = 0;
    }
//    char *strLevelFile = LEVEL_FILE;
//  if (iLevelCount < 0) {
	  iLevelCount = read_count_levels(strLevelFile);
//  }
  printf("iLevelCount: %d\n", iLevelCount);
  printf("call read_level\n");
    read_level(strLevelFile, iCurrentLevel);

  printf("finished read_level\n");
	
}

void loadLevel() {
	char strLevelFile[32];
	
	sprintf(strLevelFile, "%s0%d%s", LEVEL_FILE_PREFIX, iCurrentWorld, LEVEL_FILE_SUFFIX);
    read_level(strLevelFile, iCurrentLevel);
	
}


void update_screen_game() {
  struct Node *current = NULL;
  struct Node *deleteNode = NULL;
  
  //Handle the input
  if (iButtonFire1Down) {
	  shoot(0);
  } else if (iButtonFire2Down) {
      shoot(1);
  } else if (iButtonFire3Down) {
      shoot(2);
  }
  
  //Update the ship
  update_ship(ship);



  //Update the enemies
  current = listEnemy;
  deleteNode = NULL;
  struct Enemy *enemy;
  while(current != NULL) {
    enemy = (struct Enemy *) current->data;
	update_enemy(enemy);
	if (!enemy->isAlive) {
		deleteNode = current;
	}
    current = current->next;
  }
  if (deleteNode != NULL) {
    remove_node(&listEnemy, deleteNode);
	deleteNode = NULL;
	
	//check level complete only after the enemy Node has actually been removed, otherwise
	//it will still think that one Node remains when the list should be empty
    checkLevelComplete();
  }



//update the bullets
  current = listBullet;
  deleteNode = NULL;
  struct Bullet *bullet;
  while(current != NULL) {
    bullet = (struct Bullet *) current->data;
	update_bullet(bullet);
	if (!bullet->isAlive) {
		deleteNode = current;
	}
    current = current->next;
  }
  if (deleteNode != NULL) {
	remove_node(&listBullet, deleteNode); //bad because we have to iterate over the list twice, and can only delete
                                        //one node per loop
										//however, if we delete the node in the update loop, then it will break
										//the loop since the current node will be free'd
    deleteNode = NULL;

  }
//  printf("Total bullets: %d\n", count_list(listBullet));


  //Update the powerups
  current = listPowerup;
  deleteNode = NULL;
  struct Powerup *powerup;
  while(current != NULL) {
    powerup = (struct Powerup *) current->data;
	update_powerup(powerup);
	if (!powerup->isAlive) {
		deleteNode = current;
	}
    current = current->next;
  }
  if (deleteNode != NULL) {
    remove_node(&listPowerup, deleteNode);
	deleteNode = NULL;
  }


//Update the explosions  //Update the powerups
  current = listExplosion;
  deleteNode = NULL;
  struct Explosion *explosion;
  while(current != NULL) {
    explosion = (struct Explosion *) current->data;
	update_explosion(explosion);
	if (!explosion->isAlive) {
		deleteNode = current;
	}
    current = current->next;
  }
  if (deleteNode != NULL) {
    remove_node(&listExplosion, deleteNode);
	deleteNode = NULL;
  }




  //update background
  iBackgroundOffset += 10 * UNIT_SIZE * DELTA_TIME;
  if (iBackgroundOffset > 255) {
    iBackgroundOffset -= 256;
	updateBackgroundPattern(1);
  }

  checkCollisions();
  
  if (fKeyPressDelay > 0) {
	  fKeyPressDelay -= DELTA_TIME;
	  if (fKeyPressDelay < 0) {
		  fKeyPressDelay = 0;
	  }
  }
    
    //update time display
    updateTimeText();

}

void draw_screen_game() {
    
      struct Node *current;
    
    
	  //Draw the background
  int i, j;
//  int iBackgroundCols = SCREEN_WIDTH / 256;
//  int iBackgroundRows = SCREEN_HEIGHT / 256;
  int iBackgroundCols = BACKGROUND_COLS;
  int iBackgroundRows = BACKGROUND_ROWS;
  for (i = 0; i < iBackgroundRows; i++) {
    for (j = 0; j < iBackgroundCols; j++) {
      pos.x = j * 256;
      pos.y = (i - 1) * 256 + iBackgroundOffset;
	  pos.w = 256;
	  pos.h = 256;
		SDL_RenderCopy(renderer, imgBackground[(iCurrentWorld * 2) + iBackgroundPattern[i][j]], NULL, &pos);

    }
  }


    //Draw explosions
    current = listExplosion;
    struct Explosion *explosion;
    while(current != NULL) {
        explosion = (struct Explosion *) current->data;
        draw_explosion(explosion);
        current = current->next;
        
    }

    


  
//Draw energy meter
  SDL_Rect rectMeter;
  SDL_Color colorMeter;
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  getWeaponColor(ship->iWeaponType, &colorMeter);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
  rectMeter.x = 1000;
  rectMeter.y = 32 + 100;
  rectMeter.w = ship->fMaxEnergy;
  rectMeter.h =  16;
  SDL_RenderFillRect(renderer, &rectMeter);

//  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_SetRenderDrawColor(renderer, colorMeter.r, colorMeter.g, colorMeter.b, 255);

  rectMeter.w = ship->fEnergy;
  SDL_RenderFillRect(renderer, &rectMeter);


//Draw the score text
  pos.x = 100;
  pos.y = 32 + 32;
  
  SDL_QueryTexture(imgScoreText, NULL, NULL, &(pos.w), &(pos.h)); 
  SDL_RenderCopy(renderer, imgScoreText, NULL, &pos);

//Draw level number
  pos.x = 100;
  pos.y = 32;
  
  SDL_QueryTexture(imgLevel, NULL, NULL, &(pos.w), &(pos.h)); 
  SDL_RenderCopy(renderer, imgLevel, NULL, &pos);

//draw weapon text
  pos.x = 1000;
  pos.y = 32;
  SDL_QueryTexture(imgWeaponText, NULL, NULL, &(pos.w), &(pos.h)); 
  SDL_RenderCopy(renderer, imgWeaponText, NULL, &pos);
    
    //draw game time
    pos.x = 800;
    pos.y = 64;
    SDL_QueryTexture(imgGameTimeText, NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgGameTimeText, NULL, &pos);

//draw the fire buttons
	SDL_Texture *imgFireButton1;
	SDL_Texture *imgFireButton2;
	SDL_Texture *imgFireButton3;
	
	if (ship->iWeaponType == 0) {
		imgFireButton1 = imgFireButton[0];
		imgFireButton2 = imgFireButton[0];
		imgFireButton3 = imgFireButton[0];
	} else {
		imgFireButton1 = imgFireButton[((ship->iWeaponType - 1) * 3) + 1];
		imgFireButton2 = imgFireButton[((ship->iWeaponType - 1) * 3) + 2];
		imgFireButton3 = imgFireButton[((ship->iWeaponType - 1) * 3) + 3];
		
	}
	
	
    pos.x = 1000 + 0 * (64 + 8);
    pos.y = 64;
    SDL_QueryTexture(imgFireButton1, NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgFireButton1, NULL, &pos);

	pos.x += 4;
	pos.y += 34;
    SDL_QueryTexture(imgFireButtonText[0], NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgFireButtonText[0], NULL, &pos);


    pos.x = 1000 + 1 * (64 + 8);
    pos.y = 64;
    SDL_QueryTexture(imgFireButton2, NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgFireButton2, NULL, &pos);

	pos.x += 4;
	pos.y += 34;
    SDL_QueryTexture(imgFireButtonText[1], NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgFireButtonText[1], NULL, &pos);


    pos.x = 1000 + 2 * (64 + 8);
    pos.y = 64;
    SDL_QueryTexture(imgFireButton3, NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgFireButton3, NULL, &pos);
	
	pos.x += 4;
	pos.y += 34;
    SDL_QueryTexture(imgFireButtonText[2], NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgFireButtonText[2], NULL, &pos);

 

//END HUD DRAW 

//draw the ship
  draw_ship(ship);

 
//Draw the enemies
  current = listEnemy;
  struct Enemy *enemy;
  while(current != NULL) {
    enemy = (struct Enemy *) current->data;
	draw_enemy(enemy);
    current = current->next;
  }  
  
//Draw the powerups
  current = listPowerup;
  struct Powerup *powerup;
  while(current != NULL) {
    powerup = (struct Powerup *) current->data;
	draw_powerup(powerup);
    current = current->next;
  }  
  


//Draw the bullets
  current = listBullet;
  struct Bullet *bullet;
  while(current != NULL) {
    bullet = (struct Bullet *) current->data;
	draw_bullet(bullet);
    current = current->next;
  }


	


//Draw level complete text
  if (iLevelComplete) {
		  
    pos.x = 320;
    pos.y = 300 + (fKeyPressDelay * UNIT_SIZE);
  
    SDL_QueryTexture(imgLevelCompleteText, NULL, NULL, &(pos.w), &(pos.h)); 
    SDL_RenderCopy(renderer, imgLevelCompleteText, NULL, &pos);
  }

//Draw game over text
  if (iGameOver) {
    pos.x = 320;
    pos.y = 300 + (fKeyPressDelay * UNIT_SIZE);
  
    SDL_QueryTexture(imgGameOverText, NULL, NULL, &(pos.w), &(pos.h)); 
    SDL_RenderCopy(renderer, imgGameOverText, NULL, &pos);
  }



}


void checkCollisions() {
  int i;
  
  struct Node *currentEnemy;
  struct Enemy *enemy;

  struct Node *currentBullet;
  struct Bullet *bullet;
  
  bullet = NULL;
  
  //some room for improvement here, since we're looping over the enemy list twice
  
  //Check bullet collision
  currentBullet = listBullet;
  while (currentBullet != NULL) {
	  bullet = (struct Bullet *) currentBullet->data;
      struct Enemy *collidedEnemy = NULL;

		//bullet collision with enemy
			currentEnemy = listEnemy;
			while (currentEnemy != NULL) {
				enemy = (struct Enemy *) currentEnemy->data;



		  if ( (bullet->iHitsEnemy) &&
			(enemy->isAlive) && (bullet != NULL) && (bullet->isAlive) && 
			collidedRectRect(bullet->x, bullet->y, bullet->width, bullet->height, enemy->x, enemy->y, enemy->width, enemy->height)) {
//			  ((bullet->x + bullet->width / 2) >= enemy->x && (bullet->x + bullet->width / 2) < enemy->x + enemy->width) &&
//			  ((bullet->y + bullet->height / 2) >= enemy->y && (bullet->y + bullet->height / 2) < enemy->y + enemy->height) ) {
		    bullet->isAlive = FALSE;

			damage_enemy(enemy, 1);
              collidedEnemy = enemy;  //used to prevent the enemy from being hit twice from a blast shot
			  printf("set collided enemy\n");



			
          } 


				
				currentEnemy = currentEnemy->next;
			}


//exploding bullet			
		if (!bullet->isAlive && bullet->fBlastRadius > 0) {
			printf("Bullet blast: %f\n", bullet->fBlastRadius);
//			float fExplosionX = bullet->x - ((bullet->width + UNIT_SIZE * bullet->fBlastRadius) / 2);
//			float fExplosionY = bullet->y - ((bullet->width + UNIT_SIZE * bullet->fBlastRadius) / 2);

			float fExplosionX;
			float fExplosionY;

			if (collidedEnemy != NULL) {
						printf("collidedEnemy is NULL\n");
				fExplosionX = collidedEnemy->x + (collidedEnemy->width / 2);
				fExplosionY = collidedEnemy->y + (collidedEnemy->height / 2);
			} else {
				fExplosionX = bullet->x + (bullet->width / 2);
				fExplosionX = bullet->y + (bullet->height / 2);
			}

						printf("explosion x and y calculated\n");

			
			struct Explosion *explosion = malloc(sizeof(struct Explosion));
			explosion->c.r = 0;
			explosion->c.g = 255;
			explosion->c.b = 255;
						printf("init_explosion\n");

			init_explosion(explosion, fExplosionX, fExplosionY,
									  UNIT_SIZE * bullet->fBlastRadius);
			printf("add_node to listExplosion\n");

			add_node(&listExplosion, explosion);

			
			printf("check damaged enemies\n");

			currentEnemy = listEnemy;
			while (currentEnemy != NULL) {
				enemy = (struct Enemy *) currentEnemy->data;
				
				if (enemy->isAlive && enemy != collidedEnemy) {
					//were just checking to see if the center of the enemy is in the radius of the explosion
//					if (getDistance(fExplosionX + (explosion->fRadius), fExplosionY + (explosion->fRadius), 
	//								enemy->x + (enemy->width / 2), enemy->y + (enemy->height / 2))
//									< UNIT_SIZE * bullet->fBlastRadius) {
	
					//better circle / rectangle collision
					if (collidedCircleRect(explosion->x, explosion->y, explosion->fRadius, enemy->x, enemy->y, enemy->width, enemy->height)) {
						damage_enemy(enemy, 1);

						
					}
					
				}


				currentEnemy = currentEnemy->next;
			}
			
			printf("After circle collision\n");


			/*
			struct Bullet *explodeBullet;
					explodeBullet = malloc(sizeof(struct Bullet));
					init_bullet(explodeBullet, bullet->x - bullet->fBlastRadius, bullet->y - bullet->fBlastRadius);
					explodeBullet->width = UNIT_SIZE * bullet->fBlastRadius * 2;
					explodeBullet->height = UNIT_SIZE * bullet->fBlastRadius * 2;
					
					explodeBullet->iHitsEnemy = TRUE;
					
					add_node(&listBullet, explodeBullet);
		
//		Mix_PlayChannel(-1, soundShoot, 0);
*/

			
		}
			
			
		//bullet collision with ship
		if ( (bullet->iHitsPlayer) &&
			(bullet->isAlive) &&  (ship->isAlive) &&
			collidedRectRect(bullet->x, bullet->y, bullet->width, bullet->height, ship->x, ship->y, ship->width, ship->height)) {

//			  ((bullet->x + bullet->width / 2) >= ship->x && (bullet->x + bullet->width / 2) < ship->x + ship->width) &&
//			  ((bullet->y + bullet->height / 2) >= ship->y && (bullet->y + bullet->height / 2) < ship->y + ship->height) ) {
			ship->isAlive = FALSE;
			iGameOver = TRUE;
		    Mix_PlayChannel(-1, soundShipDead, 0);
			fKeyPressDelay = 5;

	  
		} 

			
			
			
			currentBullet = currentBullet->next;
		  
  }
  

  //Check Enemy collision
	currentEnemy = listEnemy;
	while (currentEnemy != NULL) {
		
		enemy = (struct Enemy *) currentEnemy->data;
	  
		//check collision with ship	  
		if ( (enemy->isAlive) &&  (ship->isAlive) &&
			collidedRectRect(ship->x, ship->y, ship->width, ship->height, enemy->x, enemy->y, enemy->width, enemy->height)) {
//			(ship->x >= enemy->x && ship->x < enemy->x + enemy->width) &&
//			(ship->y >= enemy->y && ship->y < enemy->y + enemy->height) ) {
		ship->isAlive = FALSE;
		iGameOver = TRUE;
		fKeyPressDelay = 5;
	  
		} 

		currentEnemy = currentEnemy->next;

    }
	

  //Check Powerup collision
    struct Node *currentPowerup;
	struct Powerup *powerup;

	currentPowerup = listPowerup;
	while (currentPowerup != NULL) {
		
		powerup = (struct Powerup *) currentPowerup->data;
	  
		//check collision with ship	  
		if ( (ship->isAlive) &&  (powerup->isAlive) &&
			collidedRectRect(powerup->x, powerup->y, powerup->width, powerup->height, ship->x, ship->y, ship->width, ship->height)) {
		
//			(ship->x >= powerup->x && ship->x < powerup->x + powerup->width) &&
//			(ship->y >= powerup->y && ship->y < powerup->y + powerup->height) ) {
//			(powerup->x + (powerup->width / 2) >= ship->x && powerup->x + (powerup->width / 2) < ship->x + ship->width) &&
//			(powerup->y + (powerup->height / 2) >= ship->y && powerup->y + (powerup->height / 2) < ship->y + ship->height) ) {
				increaseFireRate_ship(ship);
				powerup->isAlive = FALSE;
				Mix_PlayChannel(-1, soundPowerup, 0);
	  
		} 

		currentPowerup = currentPowerup->next;

    }
	
	
	

}

int collidedRectRect(float r1_x, float r1_y, float r1_w, float r1_h, float r2_x, float r2_y, float r2_w, float r2_h) {
	int iCollided = TRUE;
	
	float r1_left, r2_left;
	float r1_right, r2_right;
	float r1_top, r2_top;
	float r1_bottom, r2_bottom;
	
	r1_left = r1_x;
	r1_right = r1_x + r1_w;
	r1_top = r1_y;
	r1_bottom = r1_y + r1_h;

	r2_left = r2_x;
	r2_right = r2_x + r2_w;
	r2_top = r2_y;
	r2_bottom = r2_y + r2_h;
	
	
	if (r1_bottom <= r2_top) {
		iCollided = FALSE;
	}
	
	if (r1_top >= r2_bottom) {
		return FALSE;
	}
	
	if (r1_right <= r2_left) {
		return FALSE;
	}
	
	if (r1_left >= r2_right) {
		return FALSE;
	}

	return iCollided;
	
}

int collidedCircleRect(float c1_x, float c1_y, float c1_r, float r1_x, float r1_y, float r1_w, float r1_h) {
	int iCollided = FALSE;
	float closest_x, closest_y;
	
	if (c1_x < r1_x) {
		closest_x = r1_x;
	} else if (c1_x > r1_x + r1_w) {
		closest_x = r1_x + r1_w;
	} else { 
		closest_x = c1_x;
	}
	
	if (c1_y < r1_y) {
		closest_y = r1_y;
	} else if (c1_y > r1_y + r1_h) {
		closest_y = r1_y + r1_h;
	} else {
		closest_y = c1_y;
	}
	
	if ( pow( (closest_x - c1_x), 2) + pow( (closest_y - c1_y), 2) < pow(c1_r, 2) ) {
		iCollided = TRUE;
	}
	
	return iCollided;
}




void checkLevelComplete() {
  int iNoMoreEnemies = TRUE;
  

  int iCount;

  iCount = count_list(listEnemy);

  printf("Total enemies: %d\n", iCount);

  
  if (iCount > 0) {
	  iNoMoreEnemies = FALSE;
  }
  
  if (iNoMoreEnemies) {
	  iCurrentLevel++;
	  if (iCurrentLevel > iLevelCount) {
		  iLevelComplete = TRUE;
		  fKeyPressDelay = 5;
	  } else {
//		read_level(LEVEL_FILE, iCurrentLevel);
		loadLevel();
		
	  }

		  
  }
  
  


//  iLevelComplete = iComplete;

}


void handleInput_screen_game(int iType, int iKey) {
  float fSpeed = UNIT_SIZE * 5;

  if (iType == SDL_KEYDOWN) {
    if (iKey == SDLK_UP || iKey == SDLK_w) {
      ship->vel_y = -fSpeed;
    } else if (iKey == SDLK_DOWN || iKey == SDLK_s) {
      ship->vel_y = fSpeed;
    } else if (iKey == SDLK_LEFT || iKey == SDLK_a) {
      ship->vel_x = -fSpeed;
    } else if (iKey == SDLK_RIGHT || iKey == SDLK_d) {
      ship->vel_x = fSpeed;
    } else if (iKey == SDLK_z) {
        iButtonFire1Down = TRUE;
    } else if (iKey == SDLK_x) {
        iButtonFire2Down = TRUE;
    } else if (iKey == SDLK_c) {
        iButtonFire3Down = TRUE;

    } else if (iKey == SDLK_SPACE) {

	  if (iLevelComplete) {
		if (fKeyPressDelay <= 0) {
			iCurrentLevel = 0;
			setCurrentScreen(2);
		}
	  } else if (iGameOver) {
		  		if (fKeyPressDelay <= 0) {
                    iGameContinue = TRUE;

		  start_screen_game();
				}

	  }
    } else if (iKey == SDLK_q || iKey == SDLK_ESCAPE) {
      //iKeepLooping = FALSE;
        iTitleMenuChoice = 1;
	  setCurrentScreen(0);
    } else if (iKey == SDLK_m) {
      Mix_VolumeMusic(0);
    } else if (iKey == SDLK_TAB) {
		if (iButtonOptionDown == FALSE) {
			selectWeaponUp_ship(ship);
			updateScoreText();
			iButtonOptionDown = TRUE;
		}
	}

  }


  if (iType == SDL_KEYUP) {
    if ((iKey == SDLK_UP || iKey == SDLK_w) && ship->vel_y < 0) {
      ship->vel_y = 0;
    } else if ((iKey == SDLK_DOWN  || iKey == SDLK_s) && ship->vel_y > 0) {
      ship->vel_y = 0;
    } else if ((iKey == SDLK_LEFT || iKey == SDLK_a) && ship->vel_x < 0) {
      ship->vel_x = 0;
    } else if ((iKey == SDLK_RIGHT || iKey == SDLK_d) && ship->vel_x > 0) {
      ship->vel_x = 0;

    } else if (iKey == SDLK_z) {
            iButtonFire1Down = FALSE;
    } else if (iKey == SDLK_x) {
            iButtonFire2Down = FALSE;
    } else if (iKey == SDLK_c) {
            iButtonFire3Down = FALSE;
    } else if (iKey == SDLK_TAB) {
	  iButtonOptionDown = FALSE;
    }  
  }


}


void updateScoreText() {
	printf("updateScoreText called\n");
  SDL_Color colorText = {255, 255, 0, 0};

  //score display
  char strScore[64];
  sprintf(strScore, "Score: %d", iScore);

  printf("before sprText\n");
  
  SDL_Surface *sprText;

  sprText = TTF_RenderText_Solid(fontDefault, strScore, colorText);
  
  printf("TTF_GetError: %s\n", TTF_GetError());

  
  imgScoreText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 
  
  
  //level display
  colorText.r = 0;
  colorText.g = 255;
  colorText.b = 0;

  char strLevel[20];
  sprintf(strLevel, "Area %d", iCurrentLevel + 1);
  sprText = TTF_RenderText_Solid(fontDefault, strLevel, colorText);
  imgLevel = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

  //weapon display
//  colorText.r = 255;
//  colorText.g = 0;
//  colorText.b = 0;
  getWeaponColor(ship->iWeaponType, &colorText);

  char strWeapon[64];
  sprintf(strWeapon, "%s", strWeaponNames[ship->iWeaponType]);
  sprText = TTF_RenderText_Solid(fontDefault, strWeapon, colorText);
  imgWeaponText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText);

  //level complete display
  colorText.r = 0;
  colorText.g = 0;
  colorText.b = 255;

  sprText = TTF_RenderText_Solid(fontLarge, "LEVEL COMPLETE", colorText);
  imgLevelCompleteText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

  //game over display
  sprText = TTF_RenderText_Solid(fontLarge, "GAME OVER", colorText);
  imgGameOverText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 


  //fire button text
  colorText.r = 255;
  colorText.g = 255;
  colorText.b = 255;
  
  sprText = TTF_RenderText_Solid(fontDefault, "Z", colorText);
  imgFireButtonText[0] = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

  sprText = TTF_RenderText_Solid(fontDefault, "X", colorText);
  imgFireButtonText[1] = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

  sprText = TTF_RenderText_Solid(fontDefault, "C", colorText);
  imgFireButtonText[2] = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

	
}

void updateTimeText() {
    
      SDL_Color colorText = {255, 255, 0, 0};
      SDL_Surface *sprText;
    
    
       //time display
        time_t timeCurrent = difftime(time(NULL), timeStartGame);
 //       struct tm *timeinfo = localtime(&timeCurrent);
         colorText.r = 255;
         colorText.g = 255;
         colorText.b = 255;

         char strTime[64];
    int iSeconds = (int) timeCurrent;
         sprintf(strTime, "Time %d:%d", iSeconds / 60, iSeconds % 60);
//         sprintf(strTime, "%s %d:%d:%d", "Time", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
      //  sprintf(strTime, "%s %d", "Time", timeCurrent);

         sprText = TTF_RenderText_Solid(fontDefault, strTime, colorText);
         imgGameTimeText = SDL_CreateTextureFromSurface(renderer, sprText);
         SDL_FreeSurface(sprText);
    
}


void shoot(int iLevel) {
  struct Bullet *bullet;
	
  if (ship != NULL && (ship->fShootDelay <= 0) && ship->isAlive) {

	shoot_ship(ship, iLevel, &listBullet);
	
//    Mix_PlayChannel(-1, soundShoot, 0);
  }
}

void getWeaponColor(int iWeaponType, SDL_Color *c) {
	switch(iWeaponType) {
		case 0:
			c->r = 0x41;
			c->g = 0x92;
			c->b = 0xc3;
			break;
		case 1:
			c->r = 0xdb;
			c->g = 0x41;
			c->b = 0xc3;
			break;
		case 2:
			c->r = 0xeb;
			c->g = 0xd3;
			c->b = 0x20;
			break;
		case 3:
			c->r = 0x41;
			c->g = 0x41;
			c->b = 0xff;
			break;
		case 4:
			c->r = 0x92;
			c->g = 0x41;
			c->b = 0xf3;
			break;
		case 5:
			c->r = 0x49;
			c->g = 0xa2;
			c->b = 0x69;
			break;
	}
}

void updateBackgroundPattern(int iRowsToGenerate) {
	int i, j;
	for (i = BACKGROUND_ROWS - 1; i >= 0; i--) {
		for (j = 0; j < BACKGROUND_COLS; j++) {
			if (i >= iRowsToGenerate) {
				iBackgroundPattern[i][j] = iBackgroundPattern[i - 1][j];
			} else {
				if (rand() % 10 == 0) {
					iBackgroundPattern[i][j] = 1;
				} else {
					iBackgroundPattern[i][j] = 0;
				}
				//iBackgroundPattern[i][j] = 0;
			}
			
			
		}
	}
	
}

