//2019 Levi D. Smith - levidsmith.com

#ifndef powerup_h
#define powerup_h



struct Powerup {
  float x;
  float y;
  int width;
  int height;
  int iType;
  int isAlive;
  float fLifetime;
};

void init_powerup(struct Powerup *powerup, int init_x, int init_y, int init_iType);
void update_powerup(struct Powerup *powerup);
void draw_powerup(struct Powerup *powerup);

#endif