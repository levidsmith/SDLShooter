//2020 Levi D. Smith - levidsmith.com

#ifndef enemy_hotel_h
#define enemy_hotel_h



struct EnemyHotel {
	float fShootAngle;
	float fShootDelay;
	float fMaxShootDelay;
	float fShootSpeed;
};

void init_enemy_hotel(struct Enemy *enemy);
void update_enemy_hotel(struct Enemy *enemy);
SDL_Texture *getTexture_enemy_hotel(struct Enemy *enemy);
void damage_enemy_hotel(struct Enemy *enemy, int iDamageAmount);
void shoot_enemy_hotel(struct Enemy *enemy);

#endif