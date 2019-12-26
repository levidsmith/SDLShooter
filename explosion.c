//2019 Levi D. Smith - levidsmith.com
#include <stdio.h>

//MinGW style
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
//MacOS - XCode Style
//#include <SDL2/SDL.h>
//#include <SDL2_ttf/SDL_ttf.h>
//#include <SDL2_mixer/SDL_mixer.h>

#include "explosion.h"
#include "globals.h"

extern SDL_Renderer *renderer;
extern SDL_Texture *imgExplosion_L2_00;
extern SDL_Texture *imgExplosion_L2_01;


void init_explosion(struct Explosion *explosion, float init_x, float init_y, float init_radius) {
	explosion->x = init_x;
	explosion->y = init_y;
	explosion->fRadius = init_radius;
	explosion->isAlive = TRUE;
	explosion->fLifetime = 0;
	explosion->fMaxLifetime = 1;


}


void update_explosion(struct Explosion *explosion) {
	explosion->fLifetime += DELTA_TIME;
	
	if (explosion->fLifetime >= explosion->fMaxLifetime) {
		explosion->isAlive = FALSE;
	}


}

void draw_explosion(struct Explosion *explosion) {
	SDL_Rect rect;
	SDL_Texture *img = NULL;

	int iSpriteIndex = ((int) (explosion->fLifetime * 8)) % 2; //change sprite every 0.5 seconds
	
	
	
	if (explosion->isAlive) {
		rect.x = explosion->x;
		rect.y = explosion->y;
		rect.w = explosion->fRadius * 2;
		rect.h = explosion->fRadius * 2;
	
//		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * (1 - (explosion->fLifetime / explosion->fMaxLifetime)));
//	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

//		SDL_RenderFillRect(renderer, &rect);
		if (iSpriteIndex == 0) {
			img = imgExplosion_L2_00;
		} else if (iSpriteIndex == 1) {
			img = imgExplosion_L2_01;
		}
		
		if (img != NULL) {
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 * (-1 * (explosion->fLifetime / explosion->fMaxLifetime)));
//			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_SetTextureColorMod(img, explosion->c.r, explosion->c.g, explosion->c.b);

			SDL_SetTextureAlphaMod(img, 255 * (1 - (explosion->fLifetime / explosion->fMaxLifetime)));
			SDL_RenderCopy(renderer, img, NULL, &rect);
		}

	
//	printf("Draw the explosion\n");
    }


}
