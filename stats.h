struct Stats {
	int iScore;
	int iShotsFired[NUM_WEAPONS];
	int iShotsLanded;

	int iWorldCompleted[NUM_WORLDS];
	Uint32 iWorldTime[NUM_WORLDS];

	
};

void init_stats(struct Stats *stats);
void update_stats(struct Stats *stats);
void draw_stats(struct Stats *stats);
int getShotsFired_stats(struct Stats *stats);
int getHitRate(struct Stats *stats);
Uint32 getTotalTime_stats(struct Stats *stats);


