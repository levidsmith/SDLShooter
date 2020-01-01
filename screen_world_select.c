//2019 Levi D. Smith
#include "globals.h"
#include <stdio.h>

#include "screen_world_select.h"


//Function Prototypes
extern void setCurrentScreen(int iScreen);
extern void quit();

extern SDL_Texture *imgWorldSelectText;
extern SDL_Texture *imgWorldSelectSelectedText;


//Variables
extern SDL_Renderer* renderer;
extern TTF_Font *fontDefault;
extern TTF_Font *fontLarge;

extern SDL_Texture *imgBackground[NUM_WORLDS * 2];
extern SDL_Texture *imgWorldSelectWorldText[NUM_WORLDS];
extern int iCurrentWorld;

int iWorldSelectChoice = 0;
char *strWorldNames[NUM_WORLDS] = { "Celeritas", "Multa", "Fluctus", "Crepitus", "Gelida", "Torqueo", "Quaerere", "?" };
int iBackgroundOffsetWorldSelect;




void start_screen_world_select() {
	
  SDL_Color colorText = {0, 0, 255, 0};
  SDL_Surface *sprText;
	
  sprText = TTF_RenderText_Solid(fontLarge, "World Select", colorText);
  imgWorldSelectText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 
  
  iBackgroundOffsetWorldSelect = 0;

	


  updateText_screen_world_select();
	
}


void update_screen_world_select() {
  SDL_Surface *sprText;
  SDL_Color colorText;
  SDL_Color colorHighlight = { 0xFF, 0x00, 0x00, 0xFF };
  SDL_Color colorDefault = { 0x00, 0x00, 0xFF, 0xFF };
  
	int i;
	for (i = 0; i < NUM_WORLDS; i++) {
		if (i == iWorldSelectChoice) {
			colorText = colorHighlight;
		} else {
			colorText = colorDefault;
		}
		sprText = TTF_RenderText_Solid(fontDefault, strWorldNames[i], colorText);
		imgWorldSelectWorldText[i] = SDL_CreateTextureFromSurface(renderer, sprText);
		SDL_FreeSurface(sprText); 

	}
	
  iBackgroundOffsetWorldSelect += 1 * UNIT_SIZE * DELTA_TIME;
  if (iBackgroundOffsetWorldSelect > 255) {
    iBackgroundOffsetWorldSelect -= 256;
  }

	

}

void draw_screen_world_select() {
	SDL_Rect posTitle;
	
  //Draw the background
  int i, j;
  for (i = -1; i < (SCREEN_HEIGHT / 256) + 1; i++) {
    for (j = 0; j < SCREEN_WIDTH / 256; j++) {
      posTitle.x = j * 256;
      posTitle.y = i * 256 + iBackgroundOffsetWorldSelect;
	  posTitle.w = 256;
	  posTitle.h = 256;
      SDL_RenderCopy(renderer, imgBackground[iWorldSelectChoice * 2], NULL, &posTitle);

    }
  }
  
  //Draw the text
    posTitle.x = 320;
    posTitle.y = 100;
    SDL_QueryTexture(imgWorldSelectText, NULL, NULL, &(posTitle.w), &(posTitle.h));
    SDL_RenderCopy(renderer, imgWorldSelectText, NULL, &posTitle);


	for (i = 0; i < NUM_WORLDS; i++) {
		posTitle.x = 320;
		posTitle.y = 300 + (32 * i);
  
		SDL_QueryTexture(imgWorldSelectWorldText[i], NULL, NULL, &(posTitle.w), &(posTitle.h));
		SDL_RenderCopy(renderer, imgWorldSelectWorldText[i], NULL, &posTitle);
	}


}

void handleInput_screen_world_select(int iType, int iKey) {
  if (iType == SDL_KEYDOWN) {
    if (iKey == SDLK_SPACE) {
		iCurrentWorld = iWorldSelectChoice;
		setCurrentScreen(1);

    } else if (iKey == SDLK_UP) {
        decreaseSelectedOption_screen_world_select();

    } else if (iKey == SDLK_DOWN) {
        increaseSelectedOption_screen_world_select();
    } else if (iKey == SDLK_q || iKey == SDLK_ESCAPE) {
		setCurrentScreen(0);

	}

  }


  if (iType == SDL_KEYUP) {
  }

	
	
}

void updateText_screen_world_select() {
  SDL_Color colorText = {0, 0, 255, 0};
  SDL_Surface *sprText;
  char *strWorld;
	
  if (iWorldSelectChoice == 0) {
	  strWorld = "Moon";
  } else if (iWorldSelectChoice == 1) {
	  strWorld = "Space";
  }
	
  sprText = TTF_RenderText_Solid(fontLarge, strWorld, colorText);
  imgWorldSelectSelectedText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

}

void increaseSelectedOption_screen_world_select() {
    iWorldSelectChoice++;
    
    if (iWorldSelectChoice >= NUM_WORLDS) {
        iWorldSelectChoice = 0;
    }
	
	updateText_screen_world_select();

}

void decreaseSelectedOption_screen_world_select() {
	    iWorldSelectChoice--;


    
    if (iWorldSelectChoice < 0) {
        iWorldSelectChoice = NUM_WORLDS - 1;
    }
	updateText_screen_world_select();

}