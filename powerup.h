//2019 Levi D. Smith - levidsmith.com
struct Powerup {
  float x;
  float y;
  int width;
  int height;
  int iType;
  int isAlive;
};

void init_powerup(struct Powerup *powerup, int init_x, int init_y, int init_iType);
void update_powerup(struct Powerup *powerup);
void draw_powerup(struct Powerup *powerup);
