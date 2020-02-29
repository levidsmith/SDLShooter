//2020 Levi D. Smith
#ifndef enemy_mike_h
#define enemy_mike_h

#include <stdlib.h>
#include <stdio.h>

struct EnemyMikeHead {
	struct Enemy *body[3];
	int iVulnerable[3];
	float fRot;
	float fRotSpeed;
	float fMinRotSpeed;
	float fErrorRotSpeed;
	float fSpeed;
	float fShootDelay;
	float fMaxShootDelay;
	int iShootTimes;
};

struct EnemyMikeBody {
	struct Enemy *head;
	SDL_Texture *imgTexture;
	int iValue; //could possibly combine iValue into iIndex, but leaving separate for now
	int iIndex;
};

void init_enemy_mike(struct Enemy *enemy);
void update_enemy_mike(struct Enemy *enemy);
SDL_Texture *getTexture_enemy_mike(struct Enemy *enemy);
void destroy_enemy_mike(struct Enemy *enemy);
void damage_enemy_mike(struct Enemy *enemy, int iDamageAmount);
void check_head_kill_enemy_mike(struct Enemy *enemy);
void update_enemy_mike_body(struct Enemy *enemy);
void shoot_enemy_mike(struct Enemy *enemy);
void attack_mode_enemy_mike(struct Enemy *enemy);



#endif