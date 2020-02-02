//enemy_kilo.h
#ifndef enemy_kilo_h
#define enemy_kilo_h


#endif

struct EnemyKiloBody {
    int iValue;
    struct Enemy *head;
	struct Enemy *next;
    
};


struct EnemyKiloHead {
    int iTargetValue;
//    struct Enemy *body[4];
	struct Enemy *bodyFirst;
    SDL_Texture *imgNumberValue;

};




void init_enemy_kilo(struct Enemy *enemy);
void update_enemy_kilo(struct Enemy *enemy);
void damage_enemy_kilo(struct Enemy *enemy, int iDamageAmount);
SDL_Texture *getTexture_enemy_kilo();
void check_destroy_enemy_kilo(struct Enemy *enemy);
