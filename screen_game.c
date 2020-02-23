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
#include "stats.h"
#include "options.h"
#include "util.h"

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
extern SDL_Texture *imgFireButton[(NUM_WEAPONS * 3)];
extern SDL_Texture *imgFireButtonText[3];
extern SDL_Texture *imgButtonWeaponSwitch;
extern SDL_Texture *imgButtonWeaponSwitchText;


//VARIABLES
extern TTF_Font *fontDefault;
extern TTF_Font *fontLarge;


extern Mix_Music *musicGame[NUM_WORLDS]; 
extern SDL_Texture *imgBackground[2];
extern SDL_Texture *imgScoreText;
extern SDL_Texture *imgLevelCompleteText;
extern SDL_Texture *imgGameOverText;
extern SDL_Texture *imgLevel;
extern SDL_Texture *imgWeaponText;
extern SDL_Texture *imgGameTimeText;
extern SDL_Texture *imgHealthUnit[5];
extern SDL_Texture *imgStatsText;
extern SDL_Texture *imgBonusText[3];


extern Mix_Chunk *soundShoot;
extern Mix_Chunk *soundShipDead;
extern Mix_Chunk *soundEnemyDead;
extern Mix_Chunk *soundEnemyShoot;
extern Mix_Chunk *soundEnemyHit;
extern Mix_Chunk *soundPowerup;


SDL_Rect pos;
SDL_Rect posText;

float fVerticalAxis = 0;
float fHorizontalAxis = 0;
int iButtonFire1Down = FALSE;
int iButtonFire2Down = FALSE;
int iButtonFire3Down = FALSE;
int iButtonOptionDown = FALSE;

struct Ship *ship;
struct Node *listBullet;
struct Node *listEnemy;
struct Node *listPowerup;
struct Node *listExplosion;
struct Stats *stats;

struct Options *options;


extern int iKeepLooping;
extern int iGameContinue;
extern int iCanContinue;
extern int iTitleMenuChoice;
int iBackgroundOffset;
int iWorldComplete = FALSE;
int iGameOver = FALSE;

//int iScore;
int iCurrentLevel = 0;
int iCurrentWorld = 1;
int iLevelCount = -1;

float fKeyPressDelay = 0;


char *strWeaponNames[NUM_WEAPONS] = {"Normal", "Speed Shot", "Multi Shot", "Wave Shot", "Blast Shot", "Spin Shot", "Freeze Shot", "Seek Shot" };
int iBackgroundPattern[BACKGROUND_ROWS][BACKGROUND_COLS];

//time_t timeStartGame;
//time_t timeEndGame;
Uint32 timeStartGame;
Uint32 timeCurrent;
Uint32 timePrevious;
Uint32 timeElapsed;



void start_screen_game() {
//  printf("start\n");

  clear_list(&listBullet);
  clear_list(&listEnemy);
  clear_list(&listPowerup);
  clear_list(&listExplosion);

	if (!iGameContinue) {
		timeStartGame = SDL_GetTicks();
		timeElapsed = 0;
		iCurrentLevel = 0;
		
	}



	if (ship == NULL) {
		ship = malloc(sizeof(struct Ship));
	}
	init_ship(ship);

  
  
  	if (stats == NULL) {
		stats = malloc(sizeof(struct Stats));
		init_stats(stats);
	}

  
  /*
  stats = malloc(sizeof(struct Stats));
  init_stats(stats);
  */
  
//  printf("ship values x: %d y: %d width: %d height %d\n", ship->x, ship->y, ship->width, ship->height);

    iLevelCount = getWorldLevels(iCurrentWorld);

	printf("Spawn world %d, level %d\n", iCurrentWorld, iCurrentLevel);
	spawnLevelEnemies(iCurrentWorld, iCurrentLevel);
  
  stats->iScore = 0;
  updateDisplayText();
  updateBackgroundPattern(4);
  

  iWorldComplete = FALSE;
  iGameOver = FALSE;



    //initialize game time
//    if (&timeStartGame == NULL || !iGameContinue) {
//        printf("Initialize time\n");
//        time(&timeStartGame);
//    }
	
	timeCurrent = SDL_GetTicks();
	timePrevious = SDL_GetTicks();
	
    
    //enable continue option on title screen
    iCanContinue = TRUE;
    
//  Mix_VolumeMusic(MIX_MAX_VOLUME * 0.2);
  Mix_PlayMusic(musicGame[iCurrentWorld], -1);
  
  printf("Finished start_screen_game\n");

}

/*
void loadGameData() {
	char strLevelFile[32];
	
	
  //read level
    if (!iGameContinue) {
        iCurrentLevel = 0;
    }

	  iLevelCount = read_count_levels(strLevelFile);
//  printf("iLevelCount: %d\n", iLevelCount);
//  printf("call read_level\n");
//   read_level(strLevelFile, iCurrentLevel);
//	parse_level_data(iCurrentWorld, iCurrentLevel);

//  printf("finished read_level\n");
	
}
*/


/*
void spawnLevelEnemies(int iWorld, int iLevel) {

	char strLevelFile[32];
	
	sprintf(strLevelFile, "%s0%d%s", LEVEL_FILE_PREFIX, iCurrentWorld, LEVEL_FILE_SUFFIX);
    read_level(strLevelFile, iCurrentLevel);
	
}
*/


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
//	if (!enemy->isAlive) {
	if (canDestroy_enemy(enemy)) {
        destroy_enemy(enemy);
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
  
  
  //update time trackers
  if (!iWorldComplete && !iGameOver) {
//	printf("update %d\n", SDL_GetTicks());
	timePrevious = timeCurrent;
	timeCurrent = SDL_GetTicks();
//		printf("previous: %d, current: %d, elapsed %d\n", timePrevious, timeCurrent, timeElapsed);
	if (timePrevious > 0) {
		timeElapsed += timeCurrent - timePrevious;
	}
  }
    
    //update time display
    updateTimeText();

}

void draw_screen_game() {
    
      struct Node *current;
    
    
	  //Draw the background
  int i, j;
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

//Draw health meter    
  int x_offset = 640;
  SDL_Texture *img;
  for (i = 0; i < ship->iMaxHealth / 4; i++) {
	  if (ship->iHealth >= (i + 1) * 4) {
		  img = imgHealthUnit[4];
	  } else if (ship->iHealth < (i * 4)) {
		  img = imgHealthUnit[0];
	  } else {
		int iNumerator = ship->iHealth - (i * 4);
		  img = imgHealthUnit[iNumerator];
	  }

		pos.x = x_offset + (i * 40);
		pos.y = 64;
      SDL_QueryTexture(img, NULL, NULL, &(pos.w), &(pos.h));
	  SDL_RenderCopy(renderer, img, NULL, &pos);

  }
  


  
//Draw energy meter
  x_offset = 1000;
  SDL_Rect rectMeter;
    SDL_Color colorMeter;
  
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  getWeaponColor(ship->iWeaponType, &colorMeter);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
  rectMeter.x = x_offset;
  rectMeter.y = 32 + 100;
  rectMeter.w = ship->fMaxEnergy;
  rectMeter.h =  16;
  SDL_RenderFillRect(renderer, &rectMeter);

  SDL_SetRenderDrawColor(renderer, colorMeter.r, colorMeter.g, colorMeter.b, 255);

  rectMeter.w = ship->fEnergy;
  SDL_RenderFillRect(renderer, &rectMeter);

  
  //draw level 1 usage
  int iLength1 = getEnergyRequired(ship->iWeaponType, 0);
  if (ship->fEnergy < iLength1) {
	  iLength1 = ship->fEnergy;
  }
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x80);
  rectMeter.x = x_offset + ship->fEnergy - iLength1;
  rectMeter.y = 32 + 100 + 4;
  rectMeter.w = iLength1;
  rectMeter.h =  4;
  SDL_RenderFillRect(renderer, &rectMeter);
  
  //draw level 2 usage
  int iLength2 = getEnergyRequired(ship->iWeaponType, 1);
  if (ship->fEnergy < iLength2) {
	  iLength2 = ship->fEnergy;
  }
  SDL_SetRenderDrawColor(renderer, 0xc0, 0xc0, 0xc0, 0x80);
  rectMeter.x = x_offset + ship->fEnergy - iLength2;
  rectMeter.y = 32 + 100 + 8;
  rectMeter.w = iLength2;
  rectMeter.h =  4;
  SDL_RenderFillRect(renderer, &rectMeter);
  
  //draw level 3 usage
  int iLength3 = getEnergyRequired(ship->iWeaponType, 2);
  if (ship->fEnergy < iLength3) {
	  iLength3 = ship->fEnergy;
  }
  SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0x80);
  rectMeter.x = x_offset + ship->fEnergy - iLength3;
  rectMeter.y = 32 + 100 + 12;
  rectMeter.w = iLength3;
  rectMeter.h =  4;
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
	
//draw stats text
    pos.x = 32;
    pos.y = SCREEN_HEIGHT - 32;
    SDL_QueryTexture(imgStatsText, NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgStatsText, NULL, &pos);

//draw bonus text
	int y_offset = SCREEN_HEIGHT - 32;
	if (ship->fDefensePowerupDelay > 0) {
		pos.x = 1000;
		pos.y = y_offset;
		y_offset -= 32;
		SDL_QueryTexture(imgBonusText[0], NULL, NULL, &(pos.w), &(pos.h));
		SDL_RenderCopy(renderer, imgBonusText[0], NULL, &pos);
	}

	if (ship->fAttackPowerupDelay > 0) {
		pos.x = 1000;
		pos.y = y_offset;
		y_offset -= 32;
		SDL_QueryTexture(imgBonusText[1], NULL, NULL, &(pos.w), &(pos.h));
		SDL_RenderCopy(renderer, imgBonusText[1], NULL, &pos);
	}


//draw the fire buttons
	SDL_Texture *imgFireButton1;
	SDL_Texture *imgFireButton2;
	SDL_Texture *imgFireButton3;
	
	imgFireButton1 = imgFireButton[(ship->iWeaponType * 3) + 0];
	imgFireButton2 = imgFireButton[(ship->iWeaponType * 3) + 1];
	imgFireButton3 = imgFireButton[(ship->iWeaponType * 3) + 2];
		
	

    pos.x = 1000 + 0 * (64 + 8);
    pos.y = 64;
    SDL_QueryTexture(imgFireButton1, NULL, NULL, &(pos.w), &(pos.h));
	if (ship->fEnergy >= getEnergyRequired(ship->iWeaponType, 0)) {
		SDL_SetTextureColorMod(imgFireButton1, 255, 255, 255);
	} else {
		SDL_SetTextureColorMod(imgFireButton1, 128, 128, 128);
	}
    SDL_RenderCopy(renderer, imgFireButton1, NULL, &pos);

	pos.x += 4;
	pos.y += 34;
    SDL_QueryTexture(imgFireButtonText[0], NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgFireButtonText[0], NULL, &pos);


    pos.x = 1000 + 1 * (64 + 8);
    pos.y = 64;
    SDL_QueryTexture(imgFireButton2, NULL, NULL, &(pos.w), &(pos.h));
	if (ship->fEnergy >= getEnergyRequired(ship->iWeaponType, 1)) {
		SDL_SetTextureColorMod(imgFireButton2, 255, 255, 255);
	} else {
		SDL_SetTextureColorMod(imgFireButton2, 128, 128, 128);
	}
    SDL_RenderCopy(renderer, imgFireButton2, NULL, &pos);

	pos.x += 4;
	pos.y += 34;
    SDL_QueryTexture(imgFireButtonText[1], NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgFireButtonText[1], NULL, &pos);


    pos.x = 1000 + 2 * (64 + 8);
    pos.y = 64;
    SDL_QueryTexture(imgFireButton3, NULL, NULL, &(pos.w), &(pos.h));
	if (ship->fEnergy >= getEnergyRequired(ship->iWeaponType, 2)) {
		SDL_SetTextureColorMod(imgFireButton3, 255, 255, 255);
	} else {
		SDL_SetTextureColorMod(imgFireButton3, 128, 128, 128);
	}
    SDL_RenderCopy(renderer, imgFireButton3, NULL, &pos);
	
	pos.x += 4;
	pos.y += 34;
    SDL_QueryTexture(imgFireButtonText[2], NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgFireButtonText[2], NULL, &pos);



     //draw weapon switch button
    pos.x = 1000 + 192;
    pos.y = 32 - 4;
    SDL_QueryTexture(imgButtonWeaponSwitch, NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgButtonWeaponSwitch, NULL, &pos);
   
    pos.x = 1000 + 192 + 4;
    pos.y = 32;
    SDL_QueryTexture(imgButtonWeaponSwitchText, NULL, NULL, &(pos.w), &(pos.h));
    SDL_RenderCopy(renderer, imgButtonWeaponSwitchText, NULL, &pos);


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
  if (iWorldComplete) {

    SDL_QueryTexture(imgLevelCompleteText, NULL, NULL, &(pos.w), &(pos.h)); 
    pos.x = (SCREEN_WIDTH - pos.w) / 2;
    pos.y = 300 + (fKeyPressDelay * UNIT_SIZE);
    SDL_RenderCopy(renderer, imgLevelCompleteText, NULL, &pos);
  }

//Draw game over text
  if (iGameOver) {
	SDL_QueryTexture(imgGameOverText, NULL, NULL, &(pos.w), &(pos.h)); 
    pos.x = (SCREEN_WIDTH - pos.w) / 2;
    pos.y = 300 + (fKeyPressDelay * UNIT_SIZE);
    
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
			(enemy->isAlive) && (enemy->fIntroDelay <= 0) && (bullet != NULL) && (bullet->isAlive) && 
			collidedRectRect(bullet->x, bullet->y, bullet->width, bullet->height, enemy->x, enemy->y, enemy->width, enemy->height)) {
		    bullet->isAlive = FALSE;
			stats->iShotsLanded++;
			updateDisplayText();

			if (bullet->iWeaponType == 6) {
				freeze_enemy(enemy, bullet->iLevel, bullet->iDamage * bullet->iDamageMultiplier);
			} else {
				damage_enemy(enemy, bullet->iDamage * bullet->iDamageMultiplier);
				collidedEnemy = enemy;  //used to prevent the enemy from being hit twice from a blast shot
				printf("set collided enemy\n");
			}



			
          } 


				
				currentEnemy = currentEnemy->next;
			}


//exploding bullet			
		if (!bullet->isAlive && bullet->fBlastRadius > 0) {
			printf("Bullet blast: %f\n", bullet->fBlastRadius);

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

			
	//		printf("check damaged enemies\n");

			currentEnemy = listEnemy;
			while (currentEnemy != NULL) {
				enemy = (struct Enemy *) currentEnemy->data;
				
				if (enemy->isAlive && enemy != collidedEnemy) {
	
					//better circle / rectangle collision
					if (collidedCircleRect(explosion->x, explosion->y, explosion->fRadius, enemy->x, enemy->y, enemy->width, enemy->height)) {
						damage_enemy(enemy, 1);

						
					}
					
				}


				currentEnemy = currentEnemy->next;
			}
			
//			printf("After circle collision\n");



			
		}
			
			
		//bullet collision with ship
		if ( (bullet->iHitsPlayer) &&
			(bullet->isAlive) &&  (ship->isAlive) &&
			collidedRectRect(bullet->x, bullet->y, bullet->width, bullet->height, ship->x, ship->y, ship->width, ship->height)) {

			damage_ship(ship, 4);
			if (!ship->isAlive) {
				iGameOver = TRUE;
				fKeyPressDelay = 5;
				
			}


	  
		} 
			
			currentBullet = currentBullet->next;
		  
  }
  

  //Check Enemy collision
	currentEnemy = listEnemy;
	while (currentEnemy != NULL) {
		
		enemy = (struct Enemy *) currentEnemy->data;
	  
		//check collision with ship	  
		if ( (enemy->isAlive) && (enemy->fIntroDelay <= 0) &&  (ship->isAlive) &&
			collidedRectRect(ship->x, ship->y, ship->width, ship->height, enemy->x, enemy->y, enemy->width, enemy->height)) {
			damage_ship(ship, 8);
			if (!ship->isAlive) {
				iGameOver = TRUE;
				fKeyPressDelay = 5;
				
			}
	  
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
		
				applyPowerup_ship(ship, powerup->iType);
				powerup->isAlive = FALSE;
				Mix_PlayChannel(-1, soundPowerup, 0);
				updateDisplayText();
	  
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

//  printf("Total enemies: %d\n", iCount);

  
  if (iCount > 0) {
	  iNoMoreEnemies = FALSE;
  }
  
  if (iNoMoreEnemies) {
	  iCurrentLevel++;
	  if (iCurrentLevel > iLevelCount) {
		  iWorldComplete = TRUE;
		  stats->iWorldCompleted[iCurrentWorld] = TRUE;
		  stats->iWorldTime[iCurrentWorld] = timeElapsed;
		  fKeyPressDelay = 5;
	  } else {
//		loadLevel();
		spawnLevelEnemies(iCurrentWorld, iCurrentLevel);
		
	  }

		  
  }

}


void handleInput_screen_game(int iType, int iKey) {
  float fInputX = 0;
  float fInputY = 0;

  if (iType == SDL_KEYDOWN) {
	  
	  
	  
    if (iKey == SDLK_UP || iKey == SDLK_w) {
		fVerticalAxis = 1;
    } else if (iKey == SDLK_DOWN || iKey == SDLK_s) {
		fVerticalAxis = -1;
    }
	
	
    if (iKey == SDLK_LEFT || iKey == SDLK_a) {
		fHorizontalAxis = -1;
    } else if (iKey == SDLK_RIGHT || iKey == SDLK_d) {
		fHorizontalAxis = 1;
    }
	

	
	
	
    
	if (iKey == SDLK_z) {
        iButtonFire1Down = TRUE;
    } else if (iKey == SDLK_x) {
        iButtonFire2Down = TRUE;
    } else if (iKey == SDLK_c) {
        iButtonFire3Down = TRUE;

    } else if (iKey == SDLK_SPACE) {

	  if (iWorldComplete) {
		if (fKeyPressDelay <= 0) {
			iCurrentWorld = -1;
			iCurrentLevel = 0;
            iGameContinue = FALSE;
			setCurrentScreen(2);
		}
	  } else if (iGameOver) {
		  		if (fKeyPressDelay <= 0) {
                    iGameContinue = TRUE;

		  start_screen_game();
				}

	  }
    } else if (iKey == SDLK_q || iKey == SDLK_ESCAPE) {
		if (fKeyPressDelay <= 0 && !iGameOver && !iWorldComplete) {
			iTitleMenuChoice = 1;
			setCurrentScreen(0);
		}
    } else if (iKey == SDLK_m) {
      Mix_VolumeMusic(0);
    } else if (iKey == SDLK_TAB) {
		if (iButtonOptionDown == FALSE) {
			selectWeaponUp_ship(ship);
			updateDisplayText();
			iButtonOptionDown = TRUE;
		}
	}

  }


  if (iType == SDL_KEYUP) {
    if ((iKey == SDLK_UP || iKey == SDLK_w)) {
		if (fVerticalAxis > 0) {
			fVerticalAxis = 0;
		}

    } else if ((iKey == SDLK_DOWN  || iKey == SDLK_s)) {
		if (fVerticalAxis < 0) {
			fVerticalAxis = 0;
		}
    } else if ((iKey == SDLK_LEFT || iKey == SDLK_a)) {
		if (fHorizontalAxis < 0) {
			fHorizontalAxis = 0;
		}
    } else if ((iKey == SDLK_RIGHT || iKey == SDLK_d)) {
		if (fHorizontalAxis > 0) {
			fHorizontalAxis = 0;
		}

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

	float fMagnitude = sqrt(pow(fHorizontalAxis, 2) + pow(fVerticalAxis, 2));
	if (fMagnitude > 0) {
		fHorizontalAxis = fHorizontalAxis / fMagnitude;
		fVerticalAxis = fVerticalAxis / fMagnitude;
	}
	setVelocity_ship(ship, fHorizontalAxis, fVerticalAxis);


}


void updateDisplayText() {
//	printf("updateDisplayText called\n");
  SDL_Color colorText = {255, 255, 0, 0};

  //score display
  char strScore[64];
  sprintf(strScore, "Score: %d", stats->iScore);

//  printf("before sprText\n");
  
  SDL_Surface *sprText;
//  printf("TTF_GetError: %s\n", TTF_GetError());
	generateTextTexture(&imgScoreText, strScore, colorText, fontDefault);

  
  
  //level display
  colorText.r = 0;
  colorText.g = 255;
  colorText.b = 0;

  char strLevel[20];
  sprintf(strLevel, "Area %d", iCurrentLevel + 1);
	generateTextTexture(&imgLevel, strLevel, colorText, fontDefault);


  //weapon display
  getWeaponColor(ship->iWeaponType, &colorText);

  char strWeapon[64];
  sprintf(strWeapon, "%s", strWeaponNames[ship->iWeaponType]);
	generateTextTexture(&imgWeaponText, strWeapon, colorText, fontDefault);


  //level complete display
  colorText.r = 0;
  colorText.g = 0;
  colorText.b = 255;

	generateTextTexture(&imgLevelCompleteText, "LEVEL COMPLETE", colorText, fontLarge);


  //game over display
	generateTextTexture(&imgGameOverText, "GAME OVER", colorText, fontLarge);




  //fire button text
  colorText.r = 255;
  colorText.g = 255;
  colorText.b = 255;
  
	generateTextTexture(&imgFireButtonText[0], "Z", colorText, fontDefault);


	generateTextTexture(&imgFireButtonText[1], "X", colorText, fontDefault);

	generateTextTexture(&imgFireButtonText[2], "C", colorText, fontDefault);
    
    generateTextTexture(&imgButtonWeaponSwitchText, "tab", colorText, fontDefault);



  //stats display
  colorText.r = 255;
  colorText.g = 255;
  colorText.b = 255;
  char strStats[128];
  sprintf(strStats, "Shots Fired %d   Shots Landed %d   Hit Rate %d%%", getShotsFired_stats(stats), stats->iShotsLanded, getHitRate(stats));
	generateTextTexture(&imgStatsText, strStats, colorText, fontDefault);

	
	
  //bonus display
  if (ship->fDefensePowerupDelay > 0) {
	colorText.r = 0;
	colorText.g = 255;
	colorText.b = 0;
	char strStats[128];
	sprintf(strStats, "Defense Up");
	generateTextTexture(&imgBonusText[0], strStats, colorText, fontDefault);

	  
  }

  if (ship->fAttackPowerupDelay > 0) {
	colorText.r = 255;
	colorText.g = 0;
	colorText.b = 0;
	char strStats[128];
	sprintf(strStats, "Attack Up");
	generateTextTexture(&imgBonusText[1], strStats, colorText, fontDefault);

	  
  }


//	printf("Finished update score text\n");
	
}

void updateTimeText() {
    
      SDL_Color colorText = {255, 255, 0, 0};
      SDL_Surface *sprText;
    
    
       //time display
         colorText.r = 255;
         colorText.g = 255;
         colorText.b = 255;

         char strTime[64];
		 char strTimeValue[16];
	
		formatTime(strTimeValue, timeElapsed);
		sprintf(strTime, "Time %s", strTimeValue);
		
		if (imgGameTimeText != NULL) {
			SDL_DestroyTexture(imgGameTimeText);
		}

         sprText = TTF_RenderText_Solid(fontDefault, strTime, colorText);
         imgGameTimeText = SDL_CreateTextureFromSurface(renderer, sprText);
         SDL_FreeSurface(sprText);
    
}


void shoot(int iLevel) {
  struct Bullet *bullet;
	
  if (ship != NULL && (ship->fShootDelay <= 0) && ship->isAlive) {

	shoot_ship(ship, iLevel, &listBullet);
	
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
		case 6:
			c->r = 0x92;
			c->g = 0xd3;
			c->b = 0xff;
			break;
		case 7:
			c->r = 0xe3;
			c->g = 0x51;
			c->b = 0x00;
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
			}
			
			
		}
	}
	
}

void generateTextTexture(SDL_Texture **imgText, char *strText, SDL_Color colorText, TTF_Font *font) {
	SDL_Surface *sprText;
	
	if (font == NULL) {
		font = fontDefault;
	}

	sprText = TTF_RenderText_Solid(font, strText, colorText);

	if (*imgText != NULL) {
	  SDL_DestroyTexture(*imgText);
	}
	*imgText = SDL_CreateTextureFromSurface(renderer, sprText);
    SDL_FreeSurface(sprText); 

}


void formatTime(char *strTime, Uint32 timeValue) {
        
		int iSeconds = (int) timeValue / 1000;
		int iHundredths = (timeValue / 10) % 100;
	
         sprintf(strTime, "%d:%02d.%02d", iSeconds / 60, iSeconds % 60, iHundredths);

}
