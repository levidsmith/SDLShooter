//2019 Levi D. Smith - levidsmith.com
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <stdlib.h>
#include "linked_list.h"
#include "ship.h"
#include "bullet.h"
#include "globals.h"

extern SDL_Renderer *renderer;
extern SDL_Texture *imgShip;
extern Mix_Chunk *soundShoot;
extern Mix_Chunk *soundWeaponSelect;



void init_ship(struct Ship *ship) {
  ship->width = 64;
  ship->height = 64;
  ship->vel_x = 0;
  ship->vel_y = 0;
  ship->x = (SCREEN_WIDTH - ship->width) / 2;
  ship->y = 720 - 128;
  ship->isAlive = TRUE;
  ship->fShootDelay = 0;
  ship->fMaxShootDelay = 0.5;
  ship->fMaxEnergy = 200;
  ship->fEnergy = ship->fMaxEnergy;
  ship->iWeaponType = 0;

}


void update_ship(struct Ship *ship) {
	  //update ship
  if (ship->isAlive) {
    ship->x += ship->vel_x * DELTA_TIME;
    ship->y += ship->vel_y * DELTA_TIME;

    if (ship->x < ship->width) {
      ship->x = ship->width;
    }

    if (ship->y < ship->height) {
      ship->y = ship->height;
    }

    if (ship->x > SCREEN_WIDTH - (ship->width * 2)) {
      ship->x = SCREEN_WIDTH - (ship->width * 2);
    }

    if (ship->y > SCREEN_HEIGHT - (ship->height * 2)) {
      ship->y = SCREEN_HEIGHT - (ship->height * 2);
    }
  }
  
  if (ship->fShootDelay > 0) {
	  ship->fShootDelay -= DELTA_TIME;
	  if (ship->fShootDelay < 0) {
		  ship->fShootDelay = 0;
	  }
  }
  
  //update energy
  ship->fEnergy += 10 * DELTA_TIME;
  if (ship->fEnergy > ship->fMaxEnergy) {
	  ship->fEnergy = ship->fMaxEnergy;
  }
	
}

void draw_ship(struct Ship *ship) {
	SDL_Rect pos;
  //Draw the ship
  if (ship->isAlive) {
	pos.x = ship->x;
	pos.y = ship->y;
	pos.w = ship->width;
	pos.h = ship->height;
    SDL_RenderCopy(renderer, imgShip, NULL, &pos);
  }

	
}

void shoot_ship(struct Ship *ship, struct Node **listBullet) {
//	ship->fShootDelay = 0.5;

  if (ship->iWeaponType == 0) {

	if (ship->fEnergy >= 5) {
		struct Bullet *bullet;
		
		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;


		
		ship->fShootDelay = ship->fMaxShootDelay;
		ship->fEnergy -= 5;
		
		add_node(listBullet, bullet);

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
  } else if (ship->iWeaponType == 1) {

	if (ship->fEnergy >= 8) {
		struct Bullet *bullet;
		
		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;


		
		ship->fShootDelay = ship->fMaxShootDelay * 0.5;
		ship->fEnergy -= 5;
		
		add_node(listBullet, bullet);

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
	
	
  } else if (ship->iWeaponType == 2) {
	if (ship->fEnergy >= 15) {
		struct Bullet *bullet;
		float fVelX = 0.7071;
		float fVelY = 0.7071;
		
		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;
		add_node(listBullet, bullet);

		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_x = 5.0 * fVelX;
		bullet->vel_y = -5.0 * fVelY;
		bullet->iHitsEnemy = TRUE;
		add_node(listBullet, bullet);

		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_x = 5.0 * -fVelX;
		bullet->vel_y = -5.0 * fVelY;
		bullet->iHitsEnemy = TRUE;
		add_node(listBullet, bullet);

		
		ship->fShootDelay = ship->fMaxShootDelay;
		ship->fEnergy -= 15;
		

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
  } else if (ship->iWeaponType == 3) {
	if (ship->fEnergy >= 8) {
		struct Bullet *bullet;
		
		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;
		bullet->fWaveAmplitude = 2;


		
		ship->fShootDelay = ship->fMaxShootDelay;
		ship->fEnergy -= 8;
		
		add_node(listBullet, bullet);

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
  } else if (ship->iWeaponType == 4) {
	if (ship->fEnergy >= 16) {
		struct Bullet *bullet;
		
		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;
		bullet->fWaveAmplitude = 2;
		add_node(listBullet, bullet);


		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;
		bullet->fWaveAmplitude = -2;
		add_node(listBullet, bullet);

		
		ship->fShootDelay = ship->fMaxShootDelay;
		ship->fEnergy -= 16;
		

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
  } else if (ship->iWeaponType == 5) {
	if (ship->fEnergy >= 12) {
		struct Bullet *bullet;
		
		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;
		bullet->fBlastRadius = 2;
		add_node(listBullet, bullet);


		ship->fShootDelay = ship->fMaxShootDelay;
		ship->fEnergy -= 12;
		

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
  } else if (ship->iWeaponType == 6) {
	if (ship->fEnergy >= 24) {
		struct Bullet *bullet;
		
		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;
		bullet->fBlastRadius = 4;
		add_node(listBullet, bullet);


		ship->fShootDelay = ship->fMaxShootDelay;
		ship->fEnergy -= 12;
		

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
	  
  }
}

void increaseFireRate_ship(struct Ship *ship) {
	/*
	ship->fMaxShootDelay -= 0.1;
	if (ship->fMaxShootDelay < 0.2) {
		ship->fMaxShootDelay = 0.2;
	}
	*/
	ship->fEnergy += 50;
	if (ship->fEnergy > ship->fMaxEnergy) {
		ship->fEnergy = ship->fMaxEnergy;
	}
	
}

void selectWeaponUp_ship(struct Ship *ship) {
	ship->iWeaponType++;
	if (ship->iWeaponType > 6) {
		ship->iWeaponType = 0;
	}
	Mix_PlayChannel(-1, soundWeaponSelect, 0);

}

float getCenterX_ship(struct Ship *ship) {
	return ship->x + (ship->width / 2);
}

float getCenterY_ship(struct Ship *ship) {
	return ship->y + (ship->height / 2);
}
