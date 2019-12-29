//2019 Levi D. Smith
#include "globals.h"
#include <stdio.h>

#include "globals.h"
#include "screen_title.h"

#define NUM_MENU_OPTIONS 3

//Function Prototypes
extern void setCurrentScreen(int iScreen);
extern void quit();

//Variables
extern SDL_Renderer* renderer;
extern TTF_Font *fontDefault;
extern TTF_Font *fontLarge;

extern SDL_Texture *imgBackground;
extern SDL_Texture *imgTitleText;
extern SDL_Texture *imgCopyrightText;
extern SDL_Texture *imgTitleStartText;
extern SDL_Texture *imgTitleContinueText;
extern SDL_Texture *imgTitleQuitText;

extern Mix_Music *musicTitle;


int iBackgroundOffsetTitle;
SDL_Rect posTitle;
int iTitleMenuChoice = 0;
int iGameContinue;
int iCanContinue = FALSE;



void start_screen_title() {
  SDL_Surface *sprText;
  SDL_Color colorText = {0, 0, 255, 0};
	
  sprText = TTF_RenderText_Solid(fontLarge, "SDL SHOOTER", colorText);
  imgTitleText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 


  sprText = TTF_RenderText_Solid(fontDefault, "2019 Levi D. Smith", colorText);
  imgCopyrightText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 

    updateText_screen_title();

  Mix_PlayMusic(musicTitle, -1);
//    iTitleMenuChoice = 0;

	
}


void update_screen_title() {
	
  //update background
  iBackgroundOffsetTitle += 1 * UNIT_SIZE * DELTA_TIME;
  if (iBackgroundOffsetTitle > 255) {
    iBackgroundOffsetTitle -= 256;
  }


}

void draw_screen_title() {	  

  //Draw the background
  int i, j;
  for (i = -1; i < (SCREEN_HEIGHT / 256) + 1; i++) {
    for (j = 0; j < SCREEN_WIDTH / 256; j++) {
      posTitle.x = j * 256;
      posTitle.y = i * 256 + iBackgroundOffsetTitle;
	  posTitle.w = 256;
	  posTitle.h = 256;
      SDL_RenderCopy(renderer, imgBackground, NULL, &posTitle);

    }
  }
  
  
  //Draw title text
    posTitle.x = 320;
    posTitle.y = 300;
  
    SDL_QueryTexture(imgTitleText, NULL, NULL, &(posTitle.w), &(posTitle.h));
    SDL_RenderCopy(renderer, imgTitleText, NULL, &posTitle);


  //Draw copyright text
    posTitle.x = 320;
    posTitle.y = 400;
  
    SDL_QueryTexture(imgCopyrightText, NULL, NULL, &(posTitle.w), &(posTitle.h));
    SDL_RenderCopy(renderer, imgCopyrightText, NULL, &posTitle);

    //Draw the menu
      posTitle.x = 400;
      posTitle.y = 480;
      SDL_QueryTexture(imgTitleStartText, NULL, NULL, &(posTitle.w), &(posTitle.h));
      SDL_RenderCopy(renderer, imgTitleStartText, NULL, &posTitle);

    if (iCanContinue) {
        posTitle.x = 400;
        posTitle.y = 480 + 32;
        SDL_QueryTexture(imgTitleContinueText, NULL, NULL, &(posTitle.w), &(posTitle.h));
        SDL_RenderCopy(renderer, imgTitleContinueText, NULL, &posTitle);
    }
    
    posTitle.x = 400;
    posTitle.y = 480 + 64;
    SDL_QueryTexture(imgTitleQuitText, NULL, NULL, &(posTitle.w), &(posTitle.h));
    SDL_RenderCopy(renderer, imgTitleQuitText, NULL, &posTitle);


}

void handleInput_screen_title(int iType, int iKey) {
  if (iType == SDL_KEYDOWN) {
    if (iKey == SDLK_SPACE) {
        if (iTitleMenuChoice == 0) {
            iGameContinue = FALSE;
            setCurrentScreen(1);
        } else if (iTitleMenuChoice == 1) {
            iGameContinue = TRUE;
            setCurrentScreen(1);
        } else if (iTitleMenuChoice == 2) {
            quit();
        }
    } else if (iKey == SDLK_UP) {
        decreaseSelectedOption_screen_title();
        updateText_screen_title();
    } else if (iKey == SDLK_DOWN) {
        increaseSelectedOption_screen_title();
        updateText_screen_title();

    } else if (iKey == SDLK_q || iKey == SDLK_ESCAPE) {
      quit();
	}

  }


  if (iType == SDL_KEYUP) {
  }

	
	
}

void updateText_screen_title() {
    SDL_Color colorText;
    SDL_Color colorHighlighted = { 255, 0, 0 };
    SDL_Color colorDefault = { 128, 0, 0 };
    SDL_Surface *sprText;
    
    //Start Text
    if (iTitleMenuChoice == 0) {
        colorText = colorHighlighted;
    } else {
        colorText = colorDefault;
    }
    sprText = TTF_RenderText_Solid(fontDefault, "Start", colorText);
    imgTitleStartText = SDL_CreateTextureFromSurface(renderer, sprText);
    SDL_FreeSurface(sprText);

    //Continue Text
    if (iTitleMenuChoice == 1) {
        colorText = colorHighlighted;
    } else {
        colorText = colorDefault;
    }
    sprText = TTF_RenderText_Solid(fontDefault, "Continue", colorText);
    imgTitleContinueText = SDL_CreateTextureFromSurface(renderer, sprText);
    SDL_FreeSurface(sprText);

    //Quit Text
    if (iTitleMenuChoice == 2) {
        colorText = colorHighlighted;
    } else {
        colorText = colorDefault;
    }
    sprText = TTF_RenderText_Solid(fontDefault, "Quit", colorText);
    imgTitleQuitText = SDL_CreateTextureFromSurface(renderer, sprText);
    SDL_FreeSurface(sprText);

}

void increaseSelectedOption_screen_title() {
    iTitleMenuChoice++;
    
    if (iTitleMenuChoice == 1 && !iCanContinue) {
        iTitleMenuChoice++;
    }
    
    if (iTitleMenuChoice >= NUM_MENU_OPTIONS) {
        iTitleMenuChoice = 0;
    }
}

void decreaseSelectedOption_screen_title() {
    iTitleMenuChoice--;

    if (iTitleMenuChoice == 1 && !iCanContinue) {
        iTitleMenuChoice--;
    }

    
    if (iTitleMenuChoice < 0) {
        iTitleMenuChoice = NUM_MENU_OPTIONS - 1;
    }
}
