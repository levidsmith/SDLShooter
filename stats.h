struct Stats {
	int iScore;
	int iShotsFired[NUM_WEAPONS];
	int iShotsLanded;
	
};

void init_stats(struct Stats *stats);
void update_stats(struct Stats *stats);
void draw_stats(struct Stats *stats);
int getShotsFired_stats(struct Stats *stats);
int getHitRate(struct Stats *stats);

