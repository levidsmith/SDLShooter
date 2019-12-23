//2019 Levi D. Smith - levidsmith.com
struct Explosion {
  float x;
  float y;
  float fRadius;
  float fLifetime;
  float fMaxLifetime;
  int isAlive;
  SDL_Color c;
};



void init_explosion(struct Explosion *explosion, float init_x, float init_y, float init_radius);
void update_explosion(struct Explosion *explosion);
void draw_explosion(struct Explosion *explosion);
