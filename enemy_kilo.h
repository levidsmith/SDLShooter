//2020 Levi D. Smith - levidsmith.com

#ifndef enemy_kilo_h
#define enemy_kilo_h



struct EnemyKiloBody {
    int iValue;
    struct Enemy *head;
	struct Enemy *next;
    SDL_Texture *imgEnemyKilo_L2_00;
    SDL_Texture *imgEnemyKilo_L2_01;
    
};


struct EnemyKiloHead {
    int iTargetValue;
    int iAttackValue;
//    struct Enemy *body[4];
	struct Enemy *bodyFirst;
    SDL_Texture *imgEnemyKilo_L1_00;
    SDL_Texture *imgEnemyKilo_L1_01;

};




void init_enemy_kilo(struct Enemy *enemy);
void update_enemy_kilo(struct Enemy *enemy);
void damage_enemy_kilo(struct Enemy *enemy, int iDamageAmount);
SDL_Texture *getTexture_enemy_kilo(struct Enemy *enemy);
void check_dead_enemy_kilo(struct Enemy *enemy);
void reloadHeadTexture_enemy_kilo(struct EnemyKiloHead *enemykilohead);
void reloadBodyTexture_enemy_kilo(struct EnemyKiloBody *enemykilobody);
void destroy_enemy_kilo(struct Enemy *enemy);
void destroy_enemy_kilo_head(struct EnemyKiloHead *enemykilohead);
void destroy_enemy_kilo_body(struct EnemyKiloBody *enemykilobody);

#endif