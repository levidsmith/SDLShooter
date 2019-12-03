//2019 Levi D. Smith - levidsmith.com
struct Ship {
  float vel_x;
  float vel_y;
  int x;
  int y;
  int width;
  int height;
  int isAlive;
  float fShootDelay;
};

void init_ship(struct Ship *);
void update_ship(struct Ship *);
void draw_ship(struct Ship *);
void shoot_ship(struct Ship *);
