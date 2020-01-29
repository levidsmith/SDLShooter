#ifndef enemy_juliett_h
#define enemy_juliett_h

struct EnemyJuliett {
	float fWaitDelay;
	float fTargetXUnits;
	float fMoveXUnits;

};



#endif

void init_enemy_juliett(struct Enemy *enemy);
void update_enemy_juliett(struct Enemy *enemy);
SDL_Texture *getTexture_enemy_juliett(struct Enemy *enemy);
void damage_enemy_juliett(struct Enemy *enemy, int iDamageAmount);
void set_target_enemy_juliett(struct Enemy *enemy);

