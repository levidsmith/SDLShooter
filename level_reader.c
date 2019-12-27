//2019 Levi D. Smith - levidsmith.com
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//MinGW style
#include "SDL.h"
//MacOS - XCode Style
//#include <SDL2/SDL.h>


#include "level_reader.h"
#include "enemy.h"
#include "globals.h"

extern struct Node *listEnemy;

extern struct Node *add_node(struct Node **head, void *value);


void read_level(char *strFile, int iLevelToRead) {
  int x, y;
  int i;
  int iLevel = 0;
  int iEnemyCount = 0;
  
  int iDropCountdown = getDropCountdown();
  

  printf("Reading level\n");

  int iCols;
//  iCols = (1280 / 64) + 1;
  iCols = 21;
//  char strLine[iCols];
    char *strLine;
    //  FILE *f = fopen(strFile, "r");
  x = 0;
  y = 0;

      printf("Start Reading fileb: %s\n", strFile);
        SDL_RWops *f;
        f = SDL_RWFromFile(strFile, "r");
        Sint64 iFileSize = SDL_RWsize(f);
        printf("File size: %d\n", iFileSize);
        char strFileContents[iFileSize];
            SDL_RWread( f, strFileContents, sizeof(char), iFileSize );
        printf("*** File Contents: %s\n", strFileContents);
        
        SDL_RWclose(f);
      printf("End Reading file\n");
    
    
    
    strLine = strFileContents;
//  while (fgets(strLine, iCols, f) != NULL) {
    for (i = 0; i < iFileSize; i++) {

        //printf("strLine: %s", strLine);
//    for (i = 0; i < strlen(strLine); i++) {
		
		if (iLevel == iLevelToRead) {
	
		
			if (strLine[i] == '0') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				init_enemy(e1, x, y, 0, 1);
				e1->iPoints = 50;
				if (iDropCountdown <= 0) {
					e1->hasDrop = TRUE;
					iDropCountdown = getDropCountdown();
				} else {
					iDropCountdown--;
				}


				add_node(&listEnemy, e1);
				iEnemyCount++;

			} else 	if (strLine[i] == '1') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				init_enemy(e1, x, y, 0, 2);
				e1->iPoints = 200;
				e1->iLevel = 2;
				e1->iHealth = 3;
				if (iDropCountdown <= 0) {
					e1->hasDrop = TRUE;
					iDropCountdown = getDropCountdown();
				} else {
					iDropCountdown--;
				}


				add_node(&listEnemy, e1);
				iEnemyCount++;

			} else if (strLine[i] == '2') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				init_enemy(e1, x, y, 1, 1);
				e1->iPoints = 100;

				if (iDropCountdown <= 0) {
					e1->hasDrop = TRUE;
					iDropCountdown = 2 + getDropCountdown();
				} else {
					iDropCountdown--;
				}


				add_node(&listEnemy, e1);

				iEnemyCount++;

			} else if (strLine[i] == '3') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				init_enemy(e1, x, y, 1, 2);
				e1->iPoints = 200;
				e1->iLevel = 2;
				e1->iHealth = 2;

				if (iDropCountdown <= 0) {
					e1->hasDrop = TRUE;
					iDropCountdown = 2 + getDropCountdown();
				} else {
					iDropCountdown--;
				}


				add_node(&listEnemy, e1);

				iEnemyCount++;


			} else if (strLine[i] == '4') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				init_enemy(e1, x, y, 2, 1);
				e1->iPoints = 250;
				
				e1->iHealth = 3;

				if (iDropCountdown <= 0) {
					e1->hasDrop = TRUE;
					iDropCountdown = 2 + getDropCountdown();
				} else {
					iDropCountdown--;
				}


				add_node(&listEnemy, e1);
				iEnemyCount++;

			} else if (strLine[i] == '5') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				init_enemy(e1, x, y, 2, 2);
				e1->iPoints = 500;
				
				e1->iHealth = 5;

				if (iDropCountdown <= 0) {
					e1->hasDrop = TRUE;
					iDropCountdown = 2 + getDropCountdown();
				} else {
					iDropCountdown--;
				}


				add_node(&listEnemy, e1);
				iEnemyCount++;


			} else if (strLine[i] == '6') {
				
				int j;
				
				for (j = 0; j < 4; j++) {

					struct Enemy *e1;
					e1 = malloc(sizeof(struct Enemy));
					init_enemy(e1, x, y, 3, 1);
					e1->iPoints = 500;
					
					e1->iHealth = 5;

					if (iDropCountdown <= 0) {
						e1->hasDrop = TRUE;
						iDropCountdown = 2 + getDropCountdown();
					} else {
						iDropCountdown--;
					
					}
					e1->fLifetime = j / 2.0;


					add_node(&listEnemy, e1);
					iEnemyCount++;
				}

			} else 	if (strLine[i] == '8') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				init_enemy(e1, x, y, 4, 1);
				e1->iPoints = 150;
				e1->iHealth = 3;

				if (iDropCountdown <= 0) {
					e1->hasDrop = TRUE;
					iDropCountdown = getDropCountdown();
				} else {
					iDropCountdown--;
				}


				add_node(&listEnemy, e1);
				iEnemyCount++;


			}

  			//x = i * 64;
            x += UNIT_SIZE;
			
			if (strLine[i] == '\n') {
                x = 0;
				y += UNIT_SIZE;
 //                               memset(strLine, 0, sizeof(strLine)); //clear the character array
                             //   break; //quit reading characters which may be from old character array

			}


		} 
		
//		if (i == 0 && strLine[i] == '=') {
        if (strLine[i] == '=') {
			iLevel++;
            x = 0;
			y = 0;
			//fsbreak;  //break to prevent processing any more characters on this line

		} 
     
    }
	
	
    
//  }
//  fclose(f);
  
  printf("Enemies spawned: %d\n", iEnemyCount);

}



int read_count_levels(char *strFile) {
	int iLevelCount;
	int i;
	int iCols;
    
    SDL_RWops *f;

  iLevelCount = 0;  //this is really a count of levels after the first one (index 0)
  iCols = 64; //only need to read the first character on the line

    system("pwd");
  printf("Start Reading filea: %s\n", strFile);
    f = SDL_RWFromFile(strFile, "r");
    char strLine[iCols];
    Sint64 iFileSize = SDL_RWsize(f);
    printf("File size: %d\n", iFileSize);
    char strFileContents[iFileSize];
//  FILE *f = fopen(strFile, "r");

    
//    for(i = 0; i < SDL_RWsize(f); i++ ) {
        SDL_RWread( f, strFileContents, sizeof(char), iFileSize );
//    }
    
    printf("*** File Contents: %s\n", strFileContents);

    for (i = 0; i < iFileSize; i++) {
        if (strFileContents[i] == '=') {
            iLevelCount++;
            
        }
    }
    /*
    while (fgets(strLine, iCols, f) != NULL) {
  printf("Got a line: %s\n", strLine);
	  
    for (i = 0; i < iCols; i++) {
		if (i == 0 && strLine[i] == '=') {
			iLevelCount++;
			break;  //break to prevent processing any more characters on this line
		} 
    }
	
	
    
  }*/
//  fclose(f);
    SDL_RWclose(f);
  printf("End Reading file\n");
  
  printf("total levels: %d\n", iLevelCount);
  
  return iLevelCount;

}

int getDropCountdown() {
	int iReturn;
	iReturn = 5 + (rand() % 3);
	printf("getDropCountdown: %d\n", iReturn);
	return iReturn;
}
