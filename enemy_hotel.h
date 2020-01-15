struct EnemyHotel {
	float fShootAngle;
	float fShootDelay;
	float fMaxShootDelay;
	float fShootSpeed;
};

void init_enemy_hotel(struct Enemy *enemy);
void update_enemy_hotel(struct Enemy *enemy);
//void draw_enemy_hotel(struct Enemy *enemy);
SDL_Texture *getTexture_enemy_hotel(struct Enemy *enemy);
void damage_enemy_hotel(struct Enemy *enemy, int iDamageAmount);
void shoot_enemy_hotel(struct Enemy *enemy);
