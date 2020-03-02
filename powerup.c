//2019 Levi D. Smith - levidsmith.com
#include "globals.h"
#include <stdio.h>

#include "powerup.h"

extern SDL_Renderer *renderer;
extern SDL_Texture *imgPowerup[NUM_POWERUPS * 2];
void init_powerup(struct Powerup *powerup, int init_x, int init_y, int init_iType) {
    powerup->x = init_x;
    powerup->y = init_y;
    powerup->width = 32;
    powerup->height = 32;
    powerup->isAlive = TRUE;
    powerup->iType = init_iType;
    powerup->fLifetime = 0;

}

void update_powerup(struct Powerup *powerup) {
    powerup->y += 2 * UNIT_SIZE * DELTA_TIME;
    powerup->fLifetime += DELTA_TIME;

}

void draw_powerup(struct Powerup *powerup) {
    SDL_Rect pos;

    pos.x = powerup->x;
    pos.y = powerup->y;
    pos.h = powerup->height;
    pos.w = powerup->width;

    int iSpriteIndex = ((int)(powerup->fLifetime * 4)) % 2; //change sprite every 0.25 seconds

    SDL_RenderCopy(renderer, imgPowerup[(powerup->iType * 2) + iSpriteIndex], NULL, &pos);

}
