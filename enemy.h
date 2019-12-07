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
  float fShootDelay;
};

void init_enemy(struct Enemy *, int, int, int);
void update_enemy(struct Enemy *);
void draw_enemy(struct Enemy *);
void shoot_enemy(struct Enemy *enemy);
void setShootDelay_enemy(struct Enemy *);
