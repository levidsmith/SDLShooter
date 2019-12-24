//2019 Levi D. Smith - levidsmith.com
struct Enemy {
  int iLevel;
  int iHealth;
  float x;
  float y;
  float orig_x;
  float orig_y;
  float target_x;
  float target_y;
  int iMoveToTarget;
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
  float fWaitCountdown;
  int iPoints;
};

void init_enemy(struct Enemy *, int, int, int);
void update_enemy(struct Enemy *);
void draw_enemy(struct Enemy *);
void shoot_enemy(struct Enemy *enemy);
void setShootDelay_enemy(struct Enemy *);
void setTargetPosition_enemy(struct Enemy *enemy, float x, float y);
float getDistance(float x1, float y1, float x2, float y2);
void damage_enemy(struct Enemy *enemy, int iDamageAmount);
void destroy_enemy(struct Enemy *enemy);
