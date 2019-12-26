//2019 Levi D. Smith
#include <stdio.h>

//MinGW style
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
//MacOS - XCode Style
//#include <SDL2/SDL.h>
//#include <SDL2_ttf/SDL_ttf.h>
//#include <SDL2_mixer/SDL_mixer.h>


#include "globals.h"
#include "screen_title.h"


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
extern Mix_Music *musicTitle;


int iBackgroundOffsetTitle;
SDL_Rect posTitle;



void start_screen_title() {
  SDL_Surface *sprText;
  SDL_Color colorText = {0, 0, 255, 0};
	
  sprText = TTF_RenderText_Solid(fontLarge, "SDL SHOOTER", colorText);
  imgTitleText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 


  sprText = TTF_RenderText_Solid(fontDefault, "2019 Levi D. Smith", colorText);
  imgCopyrightText = SDL_CreateTextureFromSurface(renderer, sprText);
  SDL_FreeSurface(sprText); 


  Mix_PlayMusic(musicTitle, -1);

	
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



}

void handleInput_screen_title(int iType, int iKey) {
  if (iType == SDL_KEYDOWN) {
    if (iKey == SDLK_SPACE) {
		setCurrentScreen(1);
    } else if (iKey == SDLK_q || iKey == SDLK_ESCAPE) {
      quit();
	}

  }


  if (iType == SDL_KEYUP) {
  }

	
	
}

