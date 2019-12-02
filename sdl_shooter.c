//2019 Levi D. Smith - levidsmith.com
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "enemy.h"
#include "ship.h"
#include "level_reader.h"
#include "bullet.h"
#include "globals.h"
#include "linked_list.h"


#define MAX_ENEMIES 15

//SDL variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


SDL_Surface* screenSurface = NULL;

SDL_Texture *imgShip;
SDL_Texture *imgEnemy_00;
SDL_Texture *imgEnemy_01;
SDL_Texture *imgBackground;
SDL_Texture *imgBullet;
SDL_Texture *imgScoreText;
SDL_Texture *imgLevelCompleteText;
SDL_Texture *imgGameOverText;


TTF_Font *fontDefault;
TTF_Font *fontLarge;

Mix_Chunk *soundShoot;
Mix_Chunk *soundEnemyDead;

Mix_Music *musicLevel; 

//VARIABLES
//SDL_Rect shipPosition;
SDL_Rect pos;
SDL_Rect posText;

char strFPS[64];
Uint32 iTime;
Uint32 iDelay;
int iFrames;
int iFPS;



struct Ship *ship;
struct Enemy enemyList[MAX_ENEMIES];
//struct Bullet bullet;
struct Node *listBullet;

struct EnemyBullet {
  float x;
  float y;
  int width;
  int height;
  int isAlive;  
};

int iKeepLooping = TRUE;
int iBackgroundOffset;
int iLevelComplete = FALSE;
int iGameOver = FALSE;

int iScore;

//FUNCTION PROTOTYPES
void handleInput(int, int);
void shoot();
void checkCollisions();
//void addEnemy(struct Enemy *, int, int);
void checkLevelComplete();
void updateScoreText();

extern struct Node *add_node(struct Node **head, void *value);
extern int count_list(struct Node *head);
extern void remove_node(struct Node **head, struct Node *node);


//FUNCTIONS

void start() {
  printf("start\n");
	
  ship = malloc(sizeof(struct Ship));
  init_ship(ship);
  
  printf("ship values x: %d y: %d width: %d height %d\n", ship->x, ship->y, ship->width, ship->height);
  printf("fontDefault: %x\n", fontDefault);


//  int iSpacing = 128;
//  int x_offset = (SCREEN_WIDTH - (5 * iSpacing)) / 2;

  printf("call read_level\n");
  read_level("level_00.txt\n");

  printf("finished read_level\n");


  printf("fontDefault: %x", fontDefault);

  iScore = 0;
  updateScoreText();

  printf("finished updateScoreText\n");

  Mix_VolumeMusic(MIX_MAX_VOLUME * 0.2);
  Mix_PlayMusic(musicLevel, -1);
  printf("finished start\n");
  
}

void update() {
  int i;

   update_ship(ship);

  //Update the enemies
  for (i = 0; i < MAX_ENEMIES; i++) {
    update_enemy(&enemyList[i]);
  }


//update the bullets
  struct Node *current = listBullet;
  struct Node *deleteNode = NULL;
  struct Bullet *bullet;
  while(current != NULL) {
//	  printf("looping over bullet\n");
    bullet = (struct Bullet *) current->data;
	update_bullet(bullet);
	if (!bullet->isAlive) {
		//remove_node(&listBullet, current);
		deleteNode = current;
	}
    current = current->next;
  }
  if (deleteNode != NULL) {
	remove_node(&listBullet, deleteNode); //bad because we have to iterate over the list twice, and can only delete
                                        //one node per loop
										//however, if we delete the node in the update loop, then it will break
										//the loop since the current node will be free'd
  }
  printf("Total bullets: %d\n", count_list(listBullet));



  //update background
  iBackgroundOffset += 5 * UNIT_SIZE * DELTA_TIME;
  if (iBackgroundOffset > 255) {
    iBackgroundOffset -= 256;
  }

  checkCollisions();
}

void checkCollisions() {
  int i;
  struct Bullet *bullet;
  
  bullet = NULL;

  //Update the enemies
  for (i = 0; i < MAX_ENEMIES; i++) {
	    struct Node *current = listBullet;
		struct Bullet *bullet;
		while(current != NULL) {
			bullet = (struct Bullet *) current->data;

    if ( (enemyList[i].isAlive) && (bullet != NULL) && (bullet->isAlive) && 
         ((bullet->x + bullet->width / 2) >= enemyList[i].x && (bullet->x + bullet->width / 2) < enemyList[i].x + enemyList[i].width) &&
         ((bullet->y + bullet->height / 2) >= enemyList[i].y && (bullet->y + bullet->height / 2) < enemyList[i].y + enemyList[i].height) ) {
      bullet->isAlive = FALSE;
      enemyList[i].isAlive = FALSE;
      iScore += 100;
	  updateScoreText();
      checkLevelComplete();
      Mix_PlayChannel(-1, soundEnemyDead, 0);
    } 
			
			
			current = current->next;
		}

	  
	  
	  

    if ( (enemyList[i].isAlive) &&  (ship->isAlive) &&
         (ship->x >= enemyList[i].x && ship->x < enemyList[i].x + enemyList[i].width) &&
         (ship->y >= enemyList[i].y && ship->y < enemyList[i].y + enemyList[i].height) ) {
      ship->isAlive = FALSE;
      iGameOver = TRUE;
    } 

  }

}

void checkLevelComplete() {
  int iComplete = TRUE;

  int i;
  for (i = 0; i < MAX_ENEMIES; i++) {
    if  (enemyList[i].isAlive)  {
      iComplete = FALSE;
    }
  }

  iLevelComplete = iComplete;

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
      shoot(); 
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
//      SDL_BlitSurface(sprBackground, NULL, screenSurface, &pos);
      SDL_RenderCopy(renderer, imgBackground, NULL, &pos);

    }
  }


  //draw the ship
  draw_ship(ship);

//Draw the bullets
  struct Node *current = listBullet;
  struct Bullet *bullet;
  while(current != NULL) {
//    printf("  value: %d, address: %x\n", *((int *) current->data), current);
    bullet = (struct Bullet *) current->data;
//	printf("draw bullet at x: %d y: %d\n", bullet->x, bullet->y);
	draw_bullet(bullet);
    current = current->next;
  }
 
//Draw the enemies
  for (i = 0; i < MAX_ENEMIES; i++) {
    draw_enemy(&enemyList[i]);
  }

//Draw the score text
  pos.x = 100;
  pos.y = 100;
  
  SDL_QueryTexture(imgScoreText, NULL, NULL, &(pos.w), &(pos.h)); 
  SDL_RenderCopy(renderer, imgScoreText, NULL, &pos);

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
//  colorText.r = 255;
//  colorText.g = 255;
//  colorText.b = 0;

  char strScore[64];
  sprintf(strScore, "Score: %d", iScore);

  printf("before sprText\n");
  printf("fontDefault: %x", fontDefault);
  
  SDL_Surface *sprText;

  sprText = TTF_RenderText_Solid(fontDefault, strScore, colorText);
//  sprText = TTF_RenderText_Solid(fontDefault, strScore, colorText);
//  sprText = TTF_RenderText_Solid(fontDefault, "hello", colorText);
  
  printf("TTF_GetError: %s\n", TTF_GetError());

  
  imgScoreText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

  colorText.r = 0;
  colorText.g = 0;
  colorText.b = 255;

  sprText = TTF_RenderText_Solid(fontLarge, "LEVEL COMPLETE", colorText);
  imgLevelCompleteText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

  sprText = TTF_RenderText_Solid(fontLarge, "GAME OVER", colorText);
  imgGameOverText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 
	
}


void shoot() {
  struct Bullet *bullet;
	
  if (ship != NULL && ship->isAlive) {
	bullet = malloc(sizeof(struct Bullet));

	printf("ship at x: %d y: %d\n", ship->x, ship->y);

	init_bullet(bullet, ship->x + ship->width / 2, ship->y);
	printf("added bullet at x: %d y: %d\n", bullet->x, bullet->y);
	
	add_node(&listBullet, bullet);

    Mix_PlayChannel(-1, soundShoot, 0);
  }
}

/*
void addEnemy(struct Enemy *e, int init_x, int init_y) {
  e->x = init_x;
  e->y = init_y;
  e->vel_x = 0.2;
  e->vel_y = 0;
  e->width = 64;
  e->height = 64;
  e->fChangeMovementCountdown = 60 * 10;
  e->iType = 0;
  e->isAlive = TRUE;
}
*/



int main(int argc, char* args[]) {
//  printf("Starting game");


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
  SDL_Surface* sprEnemy_00;
  SDL_Surface* sprEnemy_01;
  SDL_Surface* sprBackground;
  SDL_Surface* sprBullet;

  
  sprShip = SDL_LoadBMP("assets/images/ship.bmp");
  SDL_SetColorKey(sprShip, SDL_TRUE, SDL_MapRGB(sprShip->format, 255, 0, 255));
  imgShip = SDL_CreateTextureFromSurface(renderer, sprShip);
  
  sprBackground = SDL_LoadBMP("assets/images/background.bmp");
  imgBackground = SDL_CreateTextureFromSurface(renderer, sprBackground);

  sprEnemy_00 = SDL_LoadBMP("assets/images/enemy.bmp");
  SDL_SetColorKey(sprEnemy_00, SDL_TRUE, SDL_MapRGB(sprEnemy_00->format, 255, 0, 255));
  imgEnemy_00 = SDL_CreateTextureFromSurface(renderer, sprEnemy_00);

  sprEnemy_01 = SDL_LoadBMP("assets/images/enemy_01_a.bmp");
  SDL_SetColorKey(sprEnemy_01, SDL_TRUE, SDL_MapRGB(sprEnemy_01->format, 255, 0, 255));
  imgEnemy_01 = SDL_CreateTextureFromSurface(renderer, sprEnemy_01);

  sprBullet = SDL_LoadBMP("assets/images/bullet.bmp");
  SDL_SetColorKey(sprBullet, SDL_TRUE, SDL_MapRGB(sprBullet->format, 255, 0, 255));
  imgBullet = SDL_CreateTextureFromSurface(renderer, sprBullet);

  SDL_FreeSurface(sprShip);
  SDL_FreeSurface(sprEnemy_00);
  SDL_FreeSurface(sprEnemy_01);
  SDL_FreeSurface(sprBackground);
  SDL_FreeSurface(sprBullet);


//handle loading fonts

  if (TTF_Init() == -1) {
    exit(1);
  }
  fontDefault = TTF_OpenFont("SudburyBasin-Regular.ttf", 20);
  fontLarge = TTF_OpenFont("SudburyBasin-Regular.ttf", 64);
  
  if (!fontDefault || !fontLarge) {
	  printf("Errors loading font\n");
  }

//  SDL_Color colorText = {255, 0, 0, 0};
//  SDL_Surface *sprText = TTF_RenderText_Solid(fontDefault, "hello", colorText);
updateScoreText();

  
//handle loading sounds
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
  soundShoot = Mix_LoadWAV("assets/audio/shoot.wav");
  soundEnemyDead = Mix_LoadWAV("assets/audio/enemy_dead.wav");

  musicLevel = Mix_LoadMUS("assets/audio/sdl-shooter-level.wav");



  start();

  iTime = SDL_GetTicks();
  
 // while (iKeepLooping == TRUE) {
	while(1) {
      SDL_Event theEvent;

//	printf("update\n");
    update();

//	printf("draw\n");
    draw();
	SDL_RenderPresent(renderer);

//    while (SDL_PollEvent(&e) != 0) {
    //while (SDL_PollEvent(&e)) {
	if (SDL_PollEvent(&theEvent)) {
			printf("SDL_PollEvent\n");
			
	//SDL_PollEvent(&e);

      if (theEvent.type  == SDL_QUIT) {
		printf("Close button pressed\n");
//        iKeepLooping = FALSE;
		printf("Stop looping\n");
		break;
		printf("Shouldn't get here\n");
      } else if (theEvent.type == SDL_KEYDOWN || theEvent.type == SDL_KEYUP) {
        handleInput(theEvent.type, theEvent.key.keysym.sym);  
      }

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

  Mix_FreeMusic(musicLevel);

  Mix_CloseAudio();

  
  SDL_DestroyTexture(imgShip);
  SDL_DestroyTexture(imgBackground);
  SDL_DestroyTexture(imgEnemy_00);
  SDL_DestroyTexture(imgEnemy_01);
  SDL_DestroyTexture(imgBullet);
  SDL_DestroyTexture(imgScoreText);
  SDL_DestroyTexture(imgLevelCompleteText);

  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
