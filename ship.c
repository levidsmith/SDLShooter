//2019 Levi D. Smith - levidsmith.com
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "linked_list.h"
#include "ship.h"
#include "bullet.h"
#include "enemy.h"
#include "stats.h"
#include "screen_game.h"

#define NUM_HEALTH_UNITS 3

extern SDL_Renderer *renderer;
extern SDL_Texture *imgShip[NUM_WEAPONS];
extern SDL_Texture *imgShipPowerup[NUM_SHIP_POWERUPS];
extern Mix_Chunk *soundShoot;
extern Mix_Chunk *soundWeaponSelect;
extern Mix_Chunk *soundShipDead;
extern Mix_Chunk *soundShipHit;

extern struct Stats *stats;





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
  ship->iMaxHealth = NUM_HEALTH_UNITS * 4;
  ship->iHealth = ship->iMaxHealth;
  ship->fInvincibleDelay = 0;
  ship->fDefensePowerupDelay = 0;
  ship->fAttackPowerupDelay = 0;
  ship->fMaxSpeed = UNIT_SIZE * 5;

printf("*** init ship complete\n");

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
  
  //invincible delay
  if (ship->fInvincibleDelay > 0) {
	ship->fInvincibleDelay -= DELTA_TIME;
	if (ship->fInvincibleDelay < 0) {
		ship->fInvincibleDelay = 0;
	}
	
  }
  
  //powerup delay
  if (ship->fDefensePowerupDelay > 0) {
	  ship->fDefensePowerupDelay -= DELTA_TIME;
	  if (ship->fDefensePowerupDelay < 0) {
		  ship->fDefensePowerupDelay = 0;
	  }
  }


  if (ship->fAttackPowerupDelay > 0) {
	  ship->fAttackPowerupDelay -= DELTA_TIME;
	  if (ship->fAttackPowerupDelay < 0) {
		  ship->fAttackPowerupDelay = 0;
	  }
  }

	
}

void draw_ship(struct Ship *ship) {
	
	
	SDL_Rect pos;
	SDL_Texture *img;
  //Draw the ship
  if (ship->isAlive) {
	pos.x = ship->x;
	pos.y = ship->y;
	pos.w = ship->width;
	pos.h = ship->height;
	
	
	img = imgShip[ship->iWeaponType];
	
	if (ship->fInvincibleDelay > 0) {
		SDL_SetTextureAlphaMod(img, 0x40);
	} else {
		SDL_SetTextureAlphaMod(img, 0xFF);
		
	}
	
    SDL_RenderCopy(renderer, img, NULL, &pos);
	
	
	if (ship->fDefensePowerupDelay > 0) {
		img = imgShipPowerup[0];
		float fFade = 128 + ( (ship->fDefensePowerupDelay - floor(ship->fDefensePowerupDelay)) * 128); 
		SDL_SetTextureAlphaMod(img, fFade);

		SDL_RenderCopy(renderer, img, NULL, &pos);
		
	}

	if (ship->fAttackPowerupDelay > 0) {
		img = imgShipPowerup[1];
		float fFade = 128 + ( (ship->fAttackPowerupDelay - floor(ship->fAttackPowerupDelay)) * 128); 
		SDL_SetTextureAlphaMod(img, fFade);

		SDL_RenderCopy(renderer, img, NULL, &pos);
		
	}
	
  }

	
}

void shoot_ship(struct Ship *ship, int iLevel, struct Node **listBullet) {
    int iEnergyRequired = 5;
	float bullet_x, bullet_y;

  if (ship->iWeaponType == 0) {

//      iEnergyRequired = 5;
	iEnergyRequired = getEnergyRequired(ship->iWeaponType, iLevel);
	if (ship->fEnergy >= iEnergyRequired) {
		struct Bullet *bullet;
		
		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;
		bullet->iWeaponType = ship->iWeaponType;


		
		ship->fShootDelay = ship->fMaxShootDelay;
		ship->fEnergy -= iEnergyRequired;
		
		add_node(listBullet, bullet);
		
		stats->iShotsFired[ship->iWeaponType] += 1;
		updateDisplayText();

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
  } else if (ship->iWeaponType == 1) {
      float fShootDelay = 0.5;
		iEnergyRequired = getEnergyRequired(ship->iWeaponType, iLevel);

      switch(iLevel) {
          case 0:
              fShootDelay = 0.3;
              break;
          case 1:
              fShootDelay = 0.2;
              break;
          case 2:
              fShootDelay = 0.1;
              break;
              
      }

	if (ship->fEnergy >= iEnergyRequired) {
		struct Bullet *bullet;
		
		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;
		bullet->iWeaponType = ship->iWeaponType;


		
		ship->fShootDelay = fShootDelay;
		ship->fEnergy -= iEnergyRequired;
		
		add_node(listBullet, bullet);

		stats->iShotsFired[ship->iWeaponType] += 1;
	updateDisplayText();

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
	
	
  } else if (ship->iWeaponType == 2) {
	  
	  	iEnergyRequired = getEnergyRequired(ship->iWeaponType, iLevel);
      
      
	if (ship->fEnergy >= iEnergyRequired) {
		struct Bullet *bullet;
        float fVelX;
        float fVelY;
        
        if (iLevel == 0) {
            fVelX = 0.5; //sin(30)
            fVelY = 0.8660; //cos(30)

            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_x = 5.0 * fVelX;
            bullet->vel_y = -5.0 * fVelY;
            bullet->iHitsEnemy = TRUE;
			bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);

            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_x = -5.0 * fVelX;
            bullet->vel_y = -5.0 * fVelY;
            bullet->iHitsEnemy = TRUE;
			bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);

			stats->iShotsFired[ship->iWeaponType] += 2;
	updateDisplayText();

            
        } else if (iLevel == 1) {
            fVelX = 0.7071; //sin(45)
            fVelY = 0.7071; //cos(45)
		
		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_y = -5;
		bullet->iHitsEnemy = TRUE;
		bullet->iWeaponType = ship->iWeaponType;
		add_node(listBullet, bullet);

		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_x = 5.0 * fVelX;
		bullet->vel_y = -5.0 * fVelY;
		bullet->iHitsEnemy = TRUE;
		bullet->iWeaponType = ship->iWeaponType;
		add_node(listBullet, bullet);

		bullet = malloc(sizeof(struct Bullet));
		init_bullet(bullet, ship->x + ship->width / 2, ship->y);
		bullet->vel_x = 5.0 * -fVelX;
		bullet->vel_y = -5.0 * fVelY;
		bullet->iHitsEnemy = TRUE;
		bullet->iWeaponType = ship->iWeaponType;
		add_node(listBullet, bullet);
		
		stats->iShotsFired[ship->iWeaponType] += 3;
	updateDisplayText();

		
        } else if (iLevel == 2) {
            
            fVelX = 0.2588; //sin(15)
            fVelY = 0.9659; //cos(15)

            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_x = 5.0 * fVelX;
            bullet->vel_y = -5.0 * fVelY;
            bullet->iHitsEnemy = TRUE;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);

            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_x = -5.0 * fVelX;
            bullet->vel_y = -5.0 * fVelY;
            bullet->iHitsEnemy = TRUE;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);

            fVelX = 0.7071; //sin(45)
            fVelY = 0.7071; //cos(45)

            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_x = 5.0 * fVelX;
            bullet->vel_y = -5.0 * fVelY;
            bullet->iHitsEnemy = TRUE;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);

            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_x = -5.0 * fVelX;
            bullet->vel_y = -5.0 * fVelY;
            bullet->iHitsEnemy = TRUE;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);
			
		stats->iShotsFired[ship->iWeaponType] += 4;
	updateDisplayText();
			

        }

		
		ship->fShootDelay = ship->fMaxShootDelay;
		ship->fEnergy -= iEnergyRequired;
		

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
  } else if (ship->iWeaponType == 3) {
	  
	  	iEnergyRequired = getEnergyRequired(ship->iWeaponType, iLevel);

      
	if (ship->fEnergy >= iEnergyRequired) {
		struct Bullet *bullet;
		
        if (iLevel == 0) {
            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_y = -5;
            bullet->iHitsEnemy = TRUE;
            bullet->fWaveAmplitude = 2;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);
			
					stats->iShotsFired[ship->iWeaponType] += 1;
	updateDisplayText();


        } else if (iLevel == 1) {
            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_y = -5;
            bullet->iHitsEnemy = TRUE;
            bullet->fWaveAmplitude = 2;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);


            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_y = -5;
            bullet->iHitsEnemy = TRUE;
            bullet->fWaveAmplitude = -2;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);
			
					stats->iShotsFired[ship->iWeaponType] += 2;
	updateDisplayText();

            
        } else if (iLevel == 2) {
            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_y = -5;
            bullet->iHitsEnemy = TRUE;
            bullet->fWaveAmplitude = 2;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);

            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_y = -5;
            bullet->iHitsEnemy = TRUE;
            bullet->fWaveAmplitude = -2;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);

            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_y = -5;
            bullet->iHitsEnemy = TRUE;
            bullet->fWaveAmplitude = 4;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);


            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_y = -5;
            bullet->iHitsEnemy = TRUE;
            bullet->fWaveAmplitude = -4;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);
			
			stats->iShotsFired[ship->iWeaponType] += 4;
	updateDisplayText();
			
        }

		
		ship->fShootDelay = ship->fMaxShootDelay;
		ship->fEnergy -= iEnergyRequired;
		

		
		Mix_PlayChannel(-1, soundShoot, 0);
	}
		
  } else if (ship->iWeaponType == 4) {
	  
	  	iEnergyRequired = getEnergyRequired(ship->iWeaponType, iLevel);

      
	if (ship->fEnergy >= iEnergyRequired) {
		struct Bullet *bullet;
		
        if (iLevel == 0) {
            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_y = -5;
            bullet->iHitsEnemy = TRUE;
            bullet->fBlastRadius = 2;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);
        } else if (iLevel == 1) {
            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_y = -5;
            bullet->iHitsEnemy = TRUE;
            bullet->fBlastRadius = 3;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);

        } else if (iLevel == 2) {
            bullet = malloc(sizeof(struct Bullet));
            init_bullet(bullet, ship->x + ship->width / 2, ship->y);
            bullet->vel_y = -5;
            bullet->iHitsEnemy = TRUE;
            bullet->fBlastRadius = 4;
		bullet->iWeaponType = ship->iWeaponType;
            add_node(listBullet, bullet);

        }
		
		stats->iShotsFired[ship->iWeaponType] += 1;
	updateDisplayText();



		ship->fShootDelay = ship->fMaxShootDelay;
		ship->fEnergy -= iEnergyRequired;
		

		
		Mix_PlayChannel(-1, soundShoot, 0);
    }
	  
  } else if (ship->iWeaponType == 5) {
	  	iEnergyRequired = getEnergyRequired(ship->iWeaponType, iLevel);

    if (ship->fEnergy >= iEnergyRequired) {
        struct Bullet *bullet;

        //release the current spin bullets
        struct Node *current = NULL;
        current = *listBullet;
        
        while(current != NULL) {
          bullet = (struct Bullet *) current->data;
          update_bullet(bullet);
          if (bullet->isSpinShot) {
              bullet->isSpinShot = FALSE;
              float    fDistance    = getDistance(ship->x, ship->y, bullet->x, bullet->y);
              bullet->vel_x = -5 * (getCenterX_ship(ship) - getCenterX_bullet(bullet)) / fDistance;
              bullet->vel_y = -5 * (getCenterY_ship(ship) - getCenterY_bullet(bullet)) / fDistance;
              
          }
          current = current->next;
        }
        
        //create the new spin bullets
        int i;
        
        if (iLevel == 0) {
            for (i = 0; i < 4; i++) {
                bullet = malloc(sizeof(struct Bullet));
                init_bullet(bullet, ship->x + ship->width / 2, ship->y);
                bullet->iHitsEnemy = TRUE;
                bullet->isSpinShot = TRUE;
                bullet->fLifetime = i * 2.0 / 4.0;
		bullet->iWeaponType = ship->iWeaponType;
                add_node(listBullet, bullet);
            }
			
					stats->iShotsFired[ship->iWeaponType] += 4;
	updateDisplayText();

			
        } else if (iLevel == 1) {
            for (i = 0; i < 8; i++) {
                bullet = malloc(sizeof(struct Bullet));
                init_bullet(bullet, ship->x + ship->width / 2, ship->y);
                bullet->iHitsEnemy = TRUE;
                bullet->isSpinShot = TRUE;
                bullet->fLifetime = i * 2.0 / 8.0;
		bullet->iWeaponType = ship->iWeaponType;
                add_node(listBullet, bullet);
            }
					stats->iShotsFired[ship->iWeaponType] += 8;
	updateDisplayText();


        } else if (iLevel == 2) {
            for (i = 0; i < 12; i++) {
                bullet = malloc(sizeof(struct Bullet));
                init_bullet(bullet, ship->x + ship->width / 2, ship->y);
                bullet->iHitsEnemy = TRUE;
                bullet->isSpinShot = TRUE;
                bullet->fLifetime = i * 2.0 / 12.0;
		bullet->iWeaponType = ship->iWeaponType;
                add_node(listBullet, bullet);
            }
			
					stats->iShotsFired[ship->iWeaponType] += 16;
	updateDisplayText();



        }
        
        



        ship->fShootDelay = ship->fMaxShootDelay;
        ship->fEnergy -= iEnergyRequired;
        

        
        Mix_PlayChannel(-1, soundShoot, 0);

    }
  }
        
    
}


void damage_ship(struct Ship *ship, int iDamage) {
	
	if (ship->fInvincibleDelay <= 0) {

		if (ship->fDefensePowerupDelay > 0) {
			ship->iHealth -= iDamage / 2;
		} else {
			ship->iHealth -= iDamage;
		}
	
		ship->fInvincibleDelay = 1;
	
		if (ship->iHealth <= 0) {
			ship->isAlive = FALSE;
		    Mix_PlayChannel(-1, soundShipDead, 0);
		} else {
		    Mix_PlayChannel(-1, soundShipHit, 0);



		}
	}
}

    

void applyPowerup_ship(struct Ship *ship, int iType) {
	
	switch(iType) {
		case 0:
			//changed powerup to give energy instead of increasing shot rate
			ship->fEnergy += 50;
			if (ship->fEnergy > ship->fMaxEnergy) {
				ship->fEnergy = ship->fMaxEnergy;
			}
			break;
		case 1:
			ship->fDefensePowerupDelay = 10;
			break;
		case 2:
			ship->iHealth += 4;
			if (ship->iHealth > ship->iMaxHealth) {
				ship->iHealth = ship->iMaxHealth;
			}
			break;
		case 3:
			ship->fAttackPowerupDelay = 10;
			break;
	}
	
}

void selectWeaponUp_ship(struct Ship *ship) {
	ship->iWeaponType++;
	if (ship->iWeaponType >= NUM_WEAPONS) {
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

int getEnergyRequired(int iWeapon, int iLevel) {
	int iEnergy = -1;
	switch(iWeapon) {
		case 0:
			switch(iLevel) {
				case 0:
				case 1:
				case 2:
					iEnergy = 5;
					break;
			}
			break;
		
		case 1:
			switch(iLevel) {
				case 0:
					iEnergy = 8;
					break;
				case 1:
					iEnergy = 12;
					break;
				case 2:
					iEnergy = 16;
					break;
			}
			break;
		case 2:
			switch(iLevel) {
				case 0:
					iEnergy = 8;
					break;
				case 1:
					iEnergy = 12;
					break;
				case 2:
					iEnergy = 16;
					break;
			}
			break;
		case 3:
			switch(iLevel) {
				case 0:
					iEnergy = 7;
					break;
				case 1:
					iEnergy = 14;
					break;
				case 2:
					iEnergy = 28;
					break;
			}
			break;
		case 4:
			switch(iLevel) {
				case 0:
					iEnergy = 12;
					break;
				case 1:
					iEnergy = 24;
					break;
				case 2:
					iEnergy = 48;
					break;
			}
			break;
		case 5:
			switch(iLevel) {
				case 0:
					iEnergy = 20;
					break;
				case 1:
					iEnergy = 40;
					break;
				case 2:
					iEnergy = 60;
					break;
			}
			break;
				
		
		
		
	}
	return iEnergy;
}


void setVelocity_ship(struct Ship *ship, float fMagnitudeX, float fMagnitudeY) {
		ship->vel_x = fMagnitudeX * ship->fMaxSpeed;
		ship->vel_y = -fMagnitudeY * ship->fMaxSpeed;
}