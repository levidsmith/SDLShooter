//2019 Levi D. Smith - levidsmith.com
struct Enemy {
  int iLevel;
  int iHealth;
  float x;
  float y;
  float orig_x;
  float orig_y;
  int width;
  int height;
  float vel_x;
  float vel_y;
  float fLifetime;
  float fChangeMovementCountdown;
  int isAlive;
  int iType;
  float fShootDelay;
  int hasDrop;
  float fDamagedCountdown;
};

void init_enemy(struct Enemy *, int, int, int);
void update_enemy(struct Enemy *);
void draw_enemy(struct Enemy *);
void shoot_enemy(struct Enemy *enemy);
void setShootDelay_enemy(struct Enemy *);
