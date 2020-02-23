//2020 Levi D. Smith
#include "util.h"

int getRandomInt(int iMin, int iMax) {
    return iMin + (rand() % (iMax - iMin));
    
}

float getDistance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}
