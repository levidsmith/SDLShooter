//2020 Levi D. Smith
#include "util.h"

int getRandomInt(int iMin, int iMax) {
    return iMin + (rand() % (iMax - iMin));
    
}

float getRandomFloat(float fMin, float fMax) {
	float f = fMin + ( (float) rand() / (float) (RAND_MAX / fMax));
	
	return f;
	
}

float getDistance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}
