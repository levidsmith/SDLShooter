//2019 Levi D. Smith
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

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


int iBackgroundOffset;
SDL_Rect pos;



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
  iBackgroundOffset += 1 * UNIT_SIZE * DELTA_TIME;
  if (iBackgroundOffset > 255) {
    iBackgroundOffset -= 256;
  }


}

void draw_screen_title() {	  

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
  
  
  //Draw title text
    pos.x = 320;
    pos.y = 300;
  
    SDL_QueryTexture(imgTitleText, NULL, NULL, &(pos.w), &(pos.h)); 
    SDL_RenderCopy(renderer, imgTitleText, NULL, &pos);


  //Draw copyright text
    pos.x = 320;
    pos.y = 400;
  
    SDL_QueryTexture(imgCopyrightText, NULL, NULL, &(pos.w), &(pos.h)); 
    SDL_RenderCopy(renderer, imgCopyrightText, NULL, &pos);



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

