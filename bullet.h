//2019 Levi D. Smith - levidsmith.com
struct Bullet {
  int x;
  int y;
  float vel_y;
  int width;
  int height;
  int isAlive;
  int iHitsPlayer;
  int iHitsEnemy;
};

void init_bullet(struct Bullet *, int, int);
void update_bullet(struct Bullet *);
void draw_bullet(struct Bullet *);