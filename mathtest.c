#include <stdio.h>
#include <math.h>

void calculateMovement(float x, float y, float target_x, float target_y) {
  float fDistanceX = target_x - x;
  float fDistanceY = target_y - y;
  float fDistance = sqrt( pow(fDistanceX, 2) + pow(fDistanceY, 2));

  printf("x: %f, y: %f, target_x: %f, target_y: %f, distance: %f\n", x, y, target_x, target_y, fDistance); 

  float fAngle = atan(fDistanceY / fDistanceX);
  printf("angle by atan: %f radians, %f degrees\n", fAngle, fAngle * 180 / 3.14159); 

  fAngle = asin(fDistanceY / fDistance);
  printf("angle by asin: %f radians, %f degrees\n", fAngle, fAngle * 180 / 3.14159); 

  fAngle = acos(fDistanceX / fDistance);
  printf("angle by acos: %f radians, %f degrees\n", fAngle, fAngle * 180 / 3.14159); 

  printf("move x: %f, y: %f\n", cos(acos(fDistanceX / fDistance)), sin(asin(fDistanceY / fDistance))); 


}

int main(void) {

  calculateMovement(0, 0, 2, 5);
  calculateMovement(2, 5, 0, 0);
  calculateMovement(50, 50, 100, 100);
  calculateMovement(150, 150, 100, 100);


  return 0;
}
