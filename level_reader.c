//2019 Levi D. Smith - levidsmith.com
#include <stdio.h>
#include <stdlib.h>

#include "level_reader.h"
#include "enemy.h"

extern struct Node *listEnemy;

extern struct Node *add_node(struct Node **head, void *value);


void read_level(char *strFile) {
  int x, y;
  int i;
  int iEnemyCount = 0;

  printf("Reading level\n");

  int iCols = (1280 / 64) + 1;
  char strLine[iCols];
  FILE *f = fopen("level_00.txt", "r");
  x = 0;
  y = 64;
  while (fgets(strLine, iCols, f) != NULL) {
    printf("strLine: %s", strLine);
    for (i = 0; i < iCols; i++) {
      if (strLine[i] == '0') {

        struct Enemy *e1;
        e1 = malloc(sizeof(struct Enemy));
        init_enemy(e1, x, y);

        e1->iType = 0;

        add_node(&listEnemy, e1);
        iEnemyCount++;
      } else if (strLine[i] == '1') {

        struct Enemy *e1;
        e1 = malloc(sizeof(struct Enemy));
        init_enemy(e1, x, y);

        e1->iType = 1;

        add_node(&listEnemy, e1);

        iEnemyCount++;



      }
     
      x += 64;
    }

    x = 0;
    y += 64;
    
  }
  fclose(f); 

}
