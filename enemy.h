//2019 Levi D. Smith - levidsmith.com
struct Enemy {
  int iHealth;
  float x;
  float y;
  int width;
  int height;
  float vel_x;
  float vel_y;
  float fLifetime;
  float fChangeMovementCountdown;
  int isAlive;
  int iType;
};

void init_enemy(struct Enemy *, int, int);
void update_enemy(struct Enemy *);
void draw_enemy(struct Enemy *);
