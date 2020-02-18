//enemy_kilo.h
#ifndef enemy_kilo_h
#define enemy_kilo_h


#endif

struct EnemyKiloBody {
    int iValue;
    struct Enemy *head;
	struct Enemy *next;
    SDL_Texture *imgEnemyKilo_L2_00;
    SDL_Texture *imgEnemyKilo_L2_01;
    
};


struct EnemyKiloHead {
    int iTargetValue;
//    struct Enemy *body[4];
	struct Enemy *bodyFirst;
    SDL_Texture *imgEnemyKilo_L1_00;
    SDL_Texture *imgEnemyKilo_L1_01;

};




void init_enemy_kilo(struct Enemy *enemy);
void update_enemy_kilo(struct Enemy *enemy);
void damage_enemy_kilo(struct Enemy *enemy, int iDamageAmount);
SDL_Texture *getTexture_enemy_kilo();
void check_destroy_enemy_kilo(struct Enemy *enemy);
void reloadBodyTexture(struct EnemyKiloBody *enemykilobody);
