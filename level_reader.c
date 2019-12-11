//2019 Levi D. Smith - levidsmith.com
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "level_reader.h"
#include "enemy.h"

extern struct Node *listEnemy;

extern struct Node *add_node(struct Node **head, void *value);


void read_level(char *strFile, int iLevelToRead) {
  int x, y;
  int i;
  int iLevel = 0;
  int iEnemyCount = 0;

  printf("Reading level\n");

  int iCols;
//  iCols = (1280 / 64) + 1;
  iCols = 21;
  char strLine[iCols];
  FILE *f = fopen("level_00.txt", "r");
  x = 0;
  y = 0;
  while (fgets(strLine, iCols, f) != NULL) {

    printf("strLine: %s", strLine);
    for (i = 0; i < strlen(strLine); i++) {
		
		if (iLevel == iLevelToRead) {
	
		
			if (strLine[i] == '0') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				init_enemy(e1, x, y, 0);


				add_node(&listEnemy, e1);
				iEnemyCount++;
			} else if (strLine[i] == '1') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				init_enemy(e1, x, y, 1);


				add_node(&listEnemy, e1);

				iEnemyCount++;
			}

  			x = i * 64;
			
			if (strLine[i] == '\n') {
				y += 64;
                                memset(strLine, 0, sizeof(strLine)); //clear the character array
                             //   break; //quit reading characters which may be from old character array

			}


		} 
		
		if (i == 0 && strLine[i] == '=') {
			iLevel++;
			y = 0;
			break;  //break to prevent processing any more characters on this line

		} 
     
    }
	
	
    
  }
  fclose(f); 
  
  printf("Enemies spawned: %d\n", iEnemyCount);

}



int read_count_levels(char *strFile) {
	int iLevelCount;
	int i;
	int iCols;

  iLevelCount = 0;  //this is really a count of levels after the first one (index 0)
  iCols = 64; //only need to read the first character on the line

  printf("Start Reading file\n");
  char strLine[iCols];
  FILE *f = fopen(strFile, "r");
  while (fgets(strLine, iCols, f) != NULL) {
  printf("Got a line: %s\n", strLine);
	  
    for (i = 0; i < iCols; i++) {
		if (i == 0 && strLine[i] == '=') {
			iLevelCount++;
			break;  //break to prevent processing any more characters on this line
		} 
    }
	
	
    
  }
  fclose(f); 
  printf("End Reading file\n");
  
  printf("total levels: %d\n", iLevelCount);
  
  return iLevelCount;

}
