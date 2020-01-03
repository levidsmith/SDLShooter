#include "globals.h"
#include "stats.h"

void init_stats(struct Stats *stats) {
	int i;
	stats->iScore = 0;
	for (i = 0; i < NUM_WEAPONS; i++) {
		stats->iShotsFired[i] = 0;
	}
	stats->iShotsLanded = 0;
	

}


void update_stats(struct Stats *stats) {

}

void draw_stats(struct Stats *stats) {

}

int getShotsFired_stats(struct Stats *stats) {
	int i;
	int iFired = 0;
	
	for (i = 0; i < NUM_WEAPONS; i++) {
		iFired += stats->iShotsFired[i];
	}
	
	return iFired;
}

int getHitRate(struct Stats *stats) {
	int iHitRate;
	int iShotsFired = getShotsFired_stats(stats);
	if (iShotsFired > 0) {
		iHitRate = 100 * stats->iShotsLanded / iShotsFired;
		
	} else {
		iHitRate = 0;
		
	}
	
	return iHitRate;
}