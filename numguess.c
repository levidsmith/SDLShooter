#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  time_t t;
  int iHiddenNumber;
  int iGuess;
  int iGuesses;

  printf("rand: %d\n", rand() % 5);
  printf("rand: %d\n", rand() % 5);
  printf("rand: %d\n", rand() % 5);
  printf("rand: %d\n", rand() % 5);
  printf("rand: %d\n", rand() % 5);
  printf("rand: %d\n", rand() % 5);
  printf("rand: %d\n", rand() % 5);
//  exit(0);

  srand(time(NULL));
  iHiddenNumber = rand();
  iHiddenNumber = 1 + (iHiddenNumber % 100);

  iGuesses = 0;
  iGuess = -1;
  while (iGuess != iHiddenNumber) {
    printf ("Guess the number between 1 and 100\n");
    scanf("%d", &iGuess);
    iGuesses++;
    if (iGuess > iHiddenNumber) {
      printf("LOWER\n");
    } else if (iGuess < iHiddenNumber) {
      printf("HIGHER\n");
    } else if (iGuess == iHiddenNumber) {
      printf("CORRECT! %d total guesses.\n", iGuesses);
    }
  }
  return 0;
}
