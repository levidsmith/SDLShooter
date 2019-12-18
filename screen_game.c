//2019 Levi D. Smith
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "globals.h"
#include "screen_game.h"
#include "linked_list.h"
#include "enemy.h"
#include "ship.h"
#include "level_reader.h"
#include "bullet.h"
#include "powerup.h"




//FUNCTION PROTOTYPES
extern struct Node *add_node(struct Node **head, void *value);
extern int count_list(struct Node *head);
extern void clear_list(struct Node **head);
extern void remove_node(struct Node **head, struct Node *node);
extern SDL_Renderer* renderer;
extern void setCurrentScreen(int iScreen);
extern void damage_enemy(struct Enemy *enemy, int iDamageAmount);


//VARIABLES
extern TTF_Font *fontDefault;
extern TTF_Font *fontLarge;


extern Mix_Music *musicGame; 
extern SDL_Texture *imgBackground;
extern SDL_Texture *imgScoreText;
extern SDL_Texture *imgLevelCompleteText;
extern SDL_Texture *imgGameOverText;
extern SDL_Texture *imgLevel;

extern Mix_Chunk *soundShoot;
extern Mix_Chunk *soundShipDead;
extern Mix_Chunk *soundEnemyDead;
extern Mix_Chunk *soundEnemyShoot;
extern Mix_Chunk *soundEnemyHit;
extern Mix_Chunk *soundPowerup;




SDL_Rect pos;
SDL_Rect posText;

int iButtonFireDown = FALSE;

struct Ship *ship;
struct Node *listBullet;
struct Node *listEnemy;
struct Node *listPowerup;


extern int iKeepLooping;
int iBackgroundOffset;
int iLevelComplete = FALSE;
int iGameOver = FALSE;

int iScore;
int iCurrentLevel = 0;
int iLevelCount = -1;

float fKeyPressDelay = 0;



void start_screen_game() {
  printf("start\n");

  clear_list(&listBullet);
  clear_list(&listEnemy);
  clear_list(&listPowerup);

	
  ship = malloc(sizeof(struct Ship));
  init_ship(ship);
  
  printf("ship values x: %d y: %d width: %d height %d\n", ship->x, ship->y, ship->width, ship->height);
  printf("fontDefault: %x\n", fontDefault);


  //read level
  if (iLevelCount < 0) {
	  iLevelCount = read_count_levels("level_00.txt");
  }
  printf("iLevelCount: %d\n", iLevelCount);
//  iLevelCount = 2;
  printf("call read_level\n");
  read_level("level_00.txt", iCurrentLevel);

  printf("finished read_level\n");
  
//  struct Powerup *powerup = malloc(sizeof(struct Powerup));
//  init_powerup(powerup, 50, 50, 0);
//  add_node(&listPowerup, powerup);
  


  printf("fontDefault: %x", fontDefault);

  iScore = 0;
  updateScoreText();
  

  iLevelComplete = FALSE;
  iGameOver = FALSE;


  printf("finished updateScoreText\n");

  Mix_VolumeMusic(MIX_MAX_VOLUME * 0.2);
  Mix_PlayMusic(musicGame, -1);
  printf("finished start\n");
  

}


void update_screen_game() {
  int i;
  struct Node *current = NULL;
  struct Node *deleteNode = NULL;
  
  //Handle the input
  if (iButtonFireDown) {
	  shoot();
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



  //update background
  iBackgroundOffset += 10 * UNIT_SIZE * DELTA_TIME;
  if (iBackgroundOffset > 255) {
    iBackgroundOffset -= 256;
  }

  checkCollisions();
  
  if (fKeyPressDelay > 0) {
	  fKeyPressDelay -= DELTA_TIME;
	  if (fKeyPressDelay < 0) {
		  fKeyPressDelay = 0;
	  }
  }

}

void draw_screen_game() {
	  //Draw the background
  int i, j;
  for (i = -1; i < (SCREEN_HEIGHT / 256) + 1; i++) {
    for (j = 0; j < SCREEN_WIDTH / 256; j++) {
      pos.x = j * 256;
      pos.y = i * 256 + iBackgroundOffset;
	  pos.w = 256;
	  pos.h = 256;
      SDL_RenderCopy(renderer, imgBackground, NULL, &pos);

    }
  }


  //draw the ship
  draw_ship(ship);

  struct Node *current;
//Draw the bullets
  current = listBullet;
  struct Bullet *bullet;
  while(current != NULL) {
    bullet = (struct Bullet *) current->data;
	draw_bullet(bullet);
    current = current->next;
  }
 
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


//Draw the score text
  pos.x = 100;
  pos.y = 100;
  
  SDL_QueryTexture(imgScoreText, NULL, NULL, &(pos.w), &(pos.h)); 
  SDL_RenderCopy(renderer, imgScoreText, NULL, &pos);

//Draw level number
  pos.x = 100;
  pos.y = 640;
  
  SDL_QueryTexture(imgLevel, NULL, NULL, &(pos.w), &(pos.h)); 
  SDL_RenderCopy(renderer, imgLevel, NULL, &pos);

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

		//bullet collision with enemy
			currentEnemy = listEnemy;
			while (currentEnemy != NULL) {
				enemy = (struct Enemy *) currentEnemy->data;



		  if ( (bullet->iHitsEnemy) &&
			(enemy->isAlive) && (bullet != NULL) && (bullet->isAlive) && 
			  ((bullet->x + bullet->width / 2) >= enemy->x && (bullet->x + bullet->width / 2) < enemy->x + enemy->width) &&
			  ((bullet->y + bullet->height / 2) >= enemy->y && (bullet->y + bullet->height / 2) < enemy->y + enemy->height) ) {
		    bullet->isAlive = FALSE;
//		    enemy->isAlive = FALSE;
			damage_enemy(enemy, 1);
//			enemy->iHealth--;
			if (!enemy->isAlive) {
//			    enemy->isAlive = FALSE;
//				enemy->fDamagedCountdown = 0.5;
				iScore += 100;
				
					    updateScoreText();
				Mix_PlayChannel(-1, soundEnemyDead, 0);
			
				if (enemy->hasDrop) {
					struct Powerup *powerup = malloc(sizeof(struct Powerup));
					init_powerup(powerup, enemy->x, enemy->y, 0);
					add_node(&listPowerup, powerup);
				}

			}
	
			
          } 


				
				currentEnemy = currentEnemy->next;
			}
			
			
		//bullet collision with ship
		if ( (bullet->iHitsPlayer) &&
			(bullet->isAlive) &&  (ship->isAlive) &&
			  ((bullet->x + bullet->width / 2) >= ship->x && (bullet->x + bullet->width / 2) < ship->x + ship->width) &&
			  ((bullet->y + bullet->height / 2) >= ship->y && (bullet->y + bullet->height / 2) < ship->y + ship->height) ) {
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
			(ship->x >= enemy->x && ship->x < enemy->x + enemy->width) &&
			(ship->y >= enemy->y && ship->y < enemy->y + enemy->height) ) {
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
//			(ship->x >= powerup->x && ship->x < powerup->x + powerup->width) &&
//			(ship->y >= powerup->y && ship->y < powerup->y + powerup->height) ) {
			(powerup->x + (powerup->width / 2) >= ship->x && powerup->x + (powerup->width / 2) < ship->x + ship->width) &&
			(powerup->y + (powerup->height / 2) >= ship->y && powerup->y + (powerup->height / 2) < ship->y + ship->height) ) {
				increaseFireRate_ship(ship);
				powerup->isAlive = FALSE;
				Mix_PlayChannel(-1, soundPowerup, 0);
	  
		} 

		currentPowerup = currentPowerup->next;

    }
	
	
	

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
		read_level("level_00.txt\n", iCurrentLevel);
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
    } else if (iKey == SDLK_SPACE) {
      //shoot(); 
	  if (iLevelComplete) {
//		  iCurrentLevel = 0;
//		  start_screen_game();
		if (fKeyPressDelay <= 0) {

    	  iCurrentLevel = 0;
			setCurrentScreen(0);
		}
	  } else if (iGameOver) {
		  		if (fKeyPressDelay <= 0) {

		  start_screen_game();
				}
	  } else {
		iButtonFireDown = TRUE;
	  }
    } else if (iKey == SDLK_q || iKey == SDLK_ESCAPE) {
      //iKeepLooping = FALSE;
	  setCurrentScreen(0);
    } else if (iKey == SDLK_m || iKey == SDLK_ESCAPE) {
      Mix_VolumeMusic(0);
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
    } else if (iKey == SDLK_SPACE) {
	  iButtonFireDown = FALSE;
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
  printf("fontDefault: %x", fontDefault);
  
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
	
}


void shoot() {
  struct Bullet *bullet;
	
  if (ship != NULL && (ship->fShootDelay <= 0) && ship->isAlive) {
	bullet = malloc(sizeof(struct Bullet));

	printf("ship at x: %d y: %d\n", ship->x, ship->y);

	init_bullet(bullet, ship->x + ship->width / 2, ship->y);

		bullet->vel_y = 5;
	bullet->iHitsEnemy = TRUE;

	printf("added bullet at x: %d y: %d\n", bullet->x, bullet->y);
	
	add_node(&listBullet, bullet);
	shoot_ship(ship);

    Mix_PlayChannel(-1, soundShoot, 0);
  }
}



