//2019 Levi D. Smith - levidsmith.com
#include "globals.h"
#include <stdio.h>

#include "powerup.h"

extern SDL_Renderer *renderer;
extern SDL_Texture *imgPowerupAlpha;


void init_powerup(struct Powerup *powerup, int init_x, int init_y, int init_iType) {
	powerup->x = init_x;
	powerup->y = init_y;
	powerup->width = 32;
	powerup->height = 32;
	powerup->isAlive = TRUE;
	
}

void update_powerup(struct Powerup *powerup) {
	powerup->y++;
	
}

void draw_powerup(struct Powerup *powerup) {
	SDL_Rect pos;
	
	pos.x = powerup->x;
	pos.y = powerup->y;
	pos.h = powerup->height;
	pos.w = powerup->width;
	
	SDL_RenderCopy(renderer, imgPowerupAlpha, NULL, &pos);
	
}
