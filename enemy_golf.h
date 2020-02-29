//2020 Levi D. Smith - levidsmith.com

#ifndef enemy_golf_h
#define enemy_golf_h



struct EnemyGolf {
	struct Enemy *previous;
	struct Enemy *next;
	int iIsHead;
	int iFoo;
	float x_min;
	float x_max;
	float y_min;
	float y_max;


};

void init_enemy_golf(struct Enemy *enemy);
void update_enemy_golf(struct Enemy *enemy);
void damage_enemy_golf(struct Enemy *enemy, int iDamageAmount);
int getIsTailDead(struct Enemy *enemy);
void updateMinMaxValues(struct Enemy *enemy);
void moveAll(struct Enemy *enemy, float x_move, float y_move);
int getCanFreeze_enemy_golf(struct Enemy *enemy);
SDL_Texture *getTexture_enemy_golf(struct Enemy *enemy);

#endif