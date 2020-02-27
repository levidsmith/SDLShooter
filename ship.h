//2019 Levi D. Smith - levidsmith.com

#ifndef ship_h
#define ship_h

struct Ship {
  float vel_x;
  float vel_y;
  int x;
  int y;
  int width;
  int height;
  int isAlive;
  float fShootDelay;
  float fMaxShootDelay;
  float fEnergy;
  float fMaxEnergy;
  int iWeaponType;
  int iHealth;
  int iMaxHealth;
  float fInvincibleDelay;
  float fDefensePowerupDelay;
  float fAttackPowerupDelay;
  float fMaxSpeed;
};

void init_ship(struct Ship *);
void update_ship(struct Ship *);
void draw_ship(struct Ship *);
void shoot_ship(struct Ship *, int iLevel, struct Node **listBullet);
void applyPowerup_ship(struct Ship *, int iType);
void selectWeaponUp_ship(struct Ship *ship);
float getCenterX_ship(struct Ship *ship);
float getCenterY_ship(struct Ship *ship);
int getEnergyRequired(int iWeapon, int iLevel);
void damage_ship(struct Ship *, int iDamage);
/*
void setAccelerationX(struct Ship *ship, float fAccel);
void setAccelerationY(struct Ship *ship, float fAccel);
*/
void setVelocity_ship(struct Ship *ship, float fMagnitudeX, float fMagnitudeY);

#endif


