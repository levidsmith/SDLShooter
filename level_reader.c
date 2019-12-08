//2019 Levi D. Smith - levidsmith.com
#include <stdio.h>
#include <stdlib.h>

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

  int iCols = (1280 / 64) + 1;
  char strLine[iCols];
  FILE *f = fopen("level_00.txt", "r");
  x = 0;
  y = 0;
  while (fgets(strLine, iCols, f) != NULL) {

	  
    printf("strLine: %s", strLine);
    for (i = 0; i < iCols; i++) {
		
		if (iLevel == iLevelToRead) {
	
		
			if (strLine[i] == '0') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				x = i * 64;
				init_enemy(e1, x, y, 0);


				add_node(&listEnemy, e1);
				iEnemyCount++;
			} else if (strLine[i] == '1') {

				struct Enemy *e1;
				e1 = malloc(sizeof(struct Enemy));
				x = i * 64;
				init_enemy(e1, x, y, 1);


				add_node(&listEnemy, e1);

				iEnemyCount++;
			}
			
			if (strLine[i] == '\n') {
				y += 64;

			}


		} 
		
		if (i == 0 && strLine[i] == '=') {
			iLevel++;
			y = 0;

		} 
		


     
    }
	
	
    
  }
  fclose(f); 

}
