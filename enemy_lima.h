//2020 Levi D. Smith
#ifndef enemy_lima_h
#define enemy_lima_h

#include <stdio.h>

struct EnemyLima {
    int isAsleep;
    float fWakeUpRadius;
    float fChaseTime;
    float fMaxChaseTime;
    float fSpeed;
    
};

#endif

void init_enemy_lima(struct Enemy *enemy);
void update_enemy_lima(struct Enemy *enemy);
SDL_Texture *getTexture_enemy_lima(struct Enemy *enemy);
void destroy_enemy_lima(struct Enemy *enemy);
