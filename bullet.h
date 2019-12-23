//2019 Levi D. Smith - levidsmith.com
struct Bullet {
  float x;
  float y;
  float vel_y;
  float vel_x;
  float orig_x;
  float orig_y;
  int width;
  int height;
  int isAlive;
  int iHitsPlayer;
  int iHitsEnemy;
  float fWaveAmplitude;
  float fLifetime;
};

void init_bullet(struct Bullet *, int, int);
void update_bullet(struct Bullet *);
void draw_bullet(struct Bullet *);