//2019 Levi D. Smith - levidsmith.com
//New stuff
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "enemy.h"
#include "ship.h"
#include "level_reader.h"
#include "bullet.h"
#include "powerup.h"
#include "globals.h"
#include "linked_list.h"

//SDL variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


SDL_Surface* screenSurface = NULL;

SDL_Texture *imgShip;
SDL_Texture *imgEnemyAlpha_00;
SDL_Texture *imgEnemyAlpha_01;
SDL_Texture *imgEnemyBravo_00;
SDL_Texture *imgEnemyBravo_01;
SDL_Texture *imgEnemyCharlie_00;
SDL_Texture *imgEnemyCharlie_01;
SDL_Texture *imgEnemyDelta_00;
SDL_Texture *imgEnemyDelta_01;


SDL_Texture *imgBackground;
SDL_Texture *imgBullet;
SDL_Texture *imgBulletEnemy;
SDL_Texture *imgScoreText;
SDL_Texture *imgLevelCompleteText;
SDL_Texture *imgGameOverText;
SDL_Texture *imgLevel;
SDL_Texture *imgPowerupAlpha;


TTF_Font *fontDefault;
TTF_Font *fontLarge;

Mix_Chunk *soundShoot;
Mix_Chunk *soundShipDead;
Mix_Chunk *soundEnemyDead;
Mix_Chunk *soundEnemyShoot;
Mix_Chunk *soundPowerup;

Mix_Music *musicLevel; 

//VARIABLES
SDL_Rect pos;
SDL_Rect posText;

char strFPS[64];
Uint32 iTime;
Uint32 iDelay;
int iFrames;
int iFPS;

int iButtonFireDown = FALSE;

struct Ship *ship;
struct Node *listBullet;
struct Node *listEnemy;
struct Node *listPowerup;


int iKeepLooping = TRUE;
int iBackgroundOffset;
int iLevelComplete = FALSE;
int iGameOver = FALSE;

int iScore;
int iCurrentLevel = 0;
int iLevelCount = -1;

//FUNCTION PROTOTYPES
void handleInput(int, int);
void shoot();
void checkCollisions();
void checkLevelComplete();
void updateScoreText();

extern struct Node *add_node(struct Node **head, void *value);
extern int count_list(struct Node *head);
extern void clear_list(struct Node **head);
extern void remove_node(struct Node **head, struct Node *node);


//FUNCTIONS

void start() {
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
  Mix_PlayMusic(musicLevel, -1);
  printf("finished start\n");
  
  
}

void update() {
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
  iBackgroundOffset += 5 * UNIT_SIZE * DELTA_TIME;
  if (iBackgroundOffset > 255) {
    iBackgroundOffset -= 256;
  }

  checkCollisions();
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
		    enemy->isAlive = FALSE;
		    iScore += 100;
		    updateScoreText();
		    Mix_PlayChannel(-1, soundEnemyDead, 0);
			
			if (enemy->hasDrop) {
				struct Powerup *powerup = malloc(sizeof(struct Powerup));
				init_powerup(powerup, enemy->x, enemy->y, 0);
				add_node(&listPowerup, powerup);
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
	  } else {
		read_level("level_00.txt\n", iCurrentLevel);
	  }

		  
  }
  
  


//  iLevelComplete = iComplete;

}


void handleInput(int iType, int iKey) {
  float fSpeed = UNIT_SIZE * 5;

  if (iType == SDL_KEYDOWN) {
    if (iKey == SDLK_UP) {
      ship->vel_y = -fSpeed;
    } else if (iKey == SDLK_DOWN) {
      ship->vel_y = fSpeed;
    } else if (iKey == SDLK_LEFT) {
      ship->vel_x = -fSpeed;
    } else if (iKey == SDLK_RIGHT) {
      ship->vel_x = fSpeed;
    } else if (iKey == SDLK_SPACE) {
      //shoot(); 
	  if (iLevelComplete) {
		  iCurrentLevel = 0;
		  start();
	  } else if (iGameOver) {
		  start();
	  } else {
		iButtonFireDown = TRUE;
	  }
    } else if (iKey == SDLK_q || iKey == SDLK_ESCAPE) {
      iKeepLooping = FALSE;
    } else if (iKey == SDLK_m || iKey == SDLK_ESCAPE) {
      Mix_VolumeMusic(0);
    }  
  }


  if (iType == SDL_KEYUP) {
    if (iKey == SDLK_UP && ship->vel_y < 0) {
      ship->vel_y = 0;
    } else if (iKey == SDLK_DOWN && ship->vel_y > 0) {
      ship->vel_y = 0;
    } else if (iKey == SDLK_LEFT && ship->vel_x < 0) {
      ship->vel_x = 0;
    } else if (iKey == SDLK_RIGHT && ship->vel_x > 0) {
      ship->vel_x = 0;
    } else if (iKey == SDLK_SPACE) {
	  iButtonFireDown = FALSE;
    }  
  }


}

void draw() {
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
    pos.y = 300;
  
    SDL_QueryTexture(imgLevelCompleteText, NULL, NULL, &(pos.w), &(pos.h)); 
    SDL_RenderCopy(renderer, imgLevelCompleteText, NULL, &pos);
  }

//Draw game over text
  if (iGameOver) {
    pos.x = 320;
    pos.y = 300;
  
    SDL_QueryTexture(imgGameOverText, NULL, NULL, &(pos.w), &(pos.h)); 
    SDL_RenderCopy(renderer, imgGameOverText, NULL, &pos);
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



int main(int argc, char* args[]) {

  //seed randomizer
  srand(time(NULL));
//  printf("random number %d\n", rand());

//  if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
  if (SDL_Init( SDL_INIT_VIDEO) < 0) {
    printf("Error: %s\n", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow("SDL Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Error: %s\n", SDL_GetError());
    return 1;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//  screenSurface = SDL_GetWindowSurface(window);




  //load media  
  SDL_Surface* sprShip;
  SDL_Surface* sprEnemy;
  SDL_Surface* sprBackground;
  SDL_Surface* sprBullet;
  SDL_Surface  *sprPowerup;

  
  sprShip = SDL_LoadBMP("assets/images/ship.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  imgShip = SDL_CreateTextureFromSurface(renderer, sprShip);
  
  sprBackground = SDL_LoadBMP("assets/images/background.bmp");
  imgBackground = SDL_CreateTextureFromSurface(renderer, sprBackground);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_alpha1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyAlpha_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_alpha2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyAlpha_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_bravo1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyBravo_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_bravo2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyBravo_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_charlie1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyCharlie_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_charlie2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyCharlie_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_delta1.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyDelta_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);

  sprEnemy = SDL_LoadBMP("assets/images/enemy_delta2.bmp");
  SDL_SetColorKey(sprEnemy, SDL_TRUE, SDL_MapRGB(sprEnemy->format, 255, 0, 255));
  imgEnemyDelta_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy);
  SDL_FreeSurface(sprEnemy);


  sprBullet = SDL_LoadBMP("assets/images/bullet.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBullet = SDL_CreateTextureFromSurface(renderer, sprBullet);

  sprBullet = SDL_LoadBMP("assets/images/bullet_enemy.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBulletEnemy = SDL_CreateTextureFromSurface(renderer, sprBullet);

  sprPowerup = SDL_LoadBMP("assets/images/powerup_alpha.bmp");
  SDL_SetColorKey(sprPowerup, SDL_TRUE, SDL_MapRGB(sprPowerup->format, 255, 0, 255));
  imgPowerupAlpha = SDL_CreateTextureFromSurface(renderer, sprPowerup);
  
  printf("created textures\n");


  SDL_FreeSurface(sprShip);
  SDL_FreeSurface(sprBackground);
  SDL_FreeSurface(sprBullet);
  SDL_FreeSurface(sprPowerup);


//handle loading fonts
  if (TTF_Init() == -1) {
    exit(1);
  }
  fontDefault = TTF_OpenFont("SudburyBasin-Regular.ttf", 20);
  fontLarge = TTF_OpenFont("SudburyBasin-Regular.ttf", 64);
  
  if (!fontDefault || !fontLarge) {
	  printf("Errors loading font\n");
  }

 
//handle loading sounds
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
  soundShoot = Mix_LoadWAV("assets/audio/shoot.wav");
  soundEnemyDead = Mix_LoadWAV("assets/audio/enemy_dead.wav");
  soundEnemyShoot = Mix_LoadWAV("assets/audio/enemy_shoot.wav");
  soundShipDead = Mix_LoadWAV("assets/audio/ship_dead.wav");
  soundPowerup = Mix_LoadWAV("assets/audio/powerup.wav");

  musicLevel = Mix_LoadMUS("assets/audio/sdl-shooter-level.wav");



  start();

  iTime = SDL_GetTicks();
  
 // while (iKeepLooping == TRUE) {
	while(1) {
      SDL_Event theEvent;

    update();

    draw();
	SDL_RenderPresent(renderer);

	if (SDL_PollEvent(&theEvent)) {
//			printf("SDL_PollEvent\n");
			

      if (theEvent.type  == SDL_QUIT) {
		printf("Close button pressed\n");
		printf("Stop looping\n");
		break;
		printf("Shouldn't get here\n");
      } else if (theEvent.type == SDL_KEYDOWN || theEvent.type == SDL_KEYUP) {
        handleInput(theEvent.type, theEvent.key.keysym.sym);  
      }

    }
	
	if (!iKeepLooping) {
		break;
	}
	

    //Set target frame rate
	/*
    iDelay = (1000 * DELTA_TIME) - (SDL_GetTicks() - iTime);
	if (iDelay < 0) {
		iDelay = 0;
	}
    SDL_Delay(iDelay);
	iFPS = 1000 / (SDL_GetTicks() - iTime);
	iTime = SDL_GetTicks();
	*/
	SDL_Delay(16);  //just set this to a constant for now.  seems to resolve freezes
  }
  
  printf("Stopped looping\n");

  TTF_Quit();


  Mix_FreeChunk(soundShoot);
  Mix_FreeChunk(soundEnemyDead);
  Mix_FreeChunk(soundEnemyShoot);
  Mix_FreeChunk(soundShipDead);
  Mix_FreeChunk(soundPowerup);

  Mix_FreeMusic(musicLevel);

  Mix_CloseAudio();

  
  SDL_DestroyTexture(imgShip);
  SDL_DestroyTexture(imgBackground);
  SDL_DestroyTexture(imgEnemyAlpha_00);
  SDL_DestroyTexture(imgEnemyAlpha_01);
  SDL_DestroyTexture(imgEnemyBravo_00);
  SDL_DestroyTexture(imgEnemyBravo_01);
  SDL_DestroyTexture(imgEnemyCharlie_00);
  SDL_DestroyTexture(imgEnemyCharlie_01);
  SDL_DestroyTexture(imgEnemyDelta_00);
  SDL_DestroyTexture(imgEnemyDelta_01);
  
  SDL_DestroyTexture(imgBullet);
  SDL_DestroyTexture(imgScoreText);
  SDL_DestroyTexture(imgLevelCompleteText);
  SDL_DestroyTexture(imgLevel);

  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
