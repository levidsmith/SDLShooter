//2019 Levi D. Smith - levidsmith.com
#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "level_reader.h"
#include "enemy.h"
#include "globals.h"

//extern struct Node *listEnemy;

extern struct Node *add_node(struct Node **head, void *value);

char *strWorldData[NUM_WORLDS];
void loadGameData() {
    int i;
    char strFile[64];

    for (i = 0; i < NUM_WORLDS; i++) {

        sprintf(strFile, "%s0%d%s", LEVEL_FILE_PREFIX, i, LEVEL_FILE_SUFFIX);
        printf("Start Reading file: %s\n", strFile);
        SDL_RWops *f;
        f = SDL_RWFromFile(strFile, "r");
        Sint64 iFileSize = SDL_RWsize(f);
//        printf("File size: %d\n", iFileSize);
        //        char strFileContents[iFileSize + 1];
        char *strFileContents;
//        printf("malloc: %d\n", (iFileSize + 1) * sizeof(char));
        strFileContents = malloc((iFileSize + 1) * sizeof(char));
        SDL_RWread(f, strFileContents, sizeof(char), iFileSize);
        //        printf("*** File Contents: %s\n", strFileContents);

        SDL_RWclose(f);
        printf("End Reading file\n");

        //	  printf("strWorldData[%d] malloc: %d\n", i, sizeof(char) * (iFileSize + 1));
        //	  strWorldData[i] = malloc(sizeof(char) * (iFileSize + 1));
        //		strcpy(strWorldData[i], strFileContents);
        strWorldData[i] = strFileContents;
        //	  strWorldData[i] = strFileContents;
//        printf("World %d - %s\n", i, strWorldData[i]);
//        printf("World 0 - %s\n", strWorldData[0]);
    }
    //     printf("World 0 - %s\n", strWorldData[0]);

}
void spawnLevelEnemies(int iWorld, int iLevelToSpawn) {
    int x, y;
    int i;
    int iLevel = 0;
    int iEnemyCount = 0;

    int iFileSize;

    int iDropCountdown = getDropCountdown();
    printf("Reading level\n");

    int iCols;
    iCols = 21;
    char *strLine;
    x = 0;
    y = 0;
    //    strLine = strFileContents;
    strLine = strWorldData[iWorld];

    printf("World size: %d, data: %s\n", strlen(strLine), strLine);
    iFileSize = strlen(strLine);
    for (i = 0; i < iFileSize; i++) {
        if (iLevel == iLevelToSpawn) {

            if (strLine[i] == '0') {
                //Alpha L1

                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_alpha, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                //				add_node(&listEnemy, e1);
                iEnemyCount++;

            } else 	if (strLine[i] == '1') {
                //Alpha L2

                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_alpha, 2, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                //				add_node(&listEnemy, e1);
                iEnemyCount++;

            } else if (strLine[i] == '2') {
                //Bravo L1

                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_bravo, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = 2 + getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                //				add_node(&listEnemy, e1);

                iEnemyCount++;

            } else if (strLine[i] == '3') {
                //Bravo L2

                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_bravo, 2, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = 2 + getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                //				add_node(&listEnemy, e1);

                iEnemyCount++;

            } else if (strLine[i] == '4') {
                //Charlie L1

                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_charlie, 1, FALSE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = 2 + getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                //				add_node(&listEnemy, e1);
                iEnemyCount++;

            } else if (strLine[i] == '5') {
                //Charlie L2

                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_charlie, 2, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = 2 + getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                //				add_node(&listEnemy, e1);
                iEnemyCount++;
            } else if (strLine[i] == '6') {
                //Delta L1
                int j;

                for (j = 0; j < 4; j++) {

                    struct Enemy *e1;
                    e1 = malloc(sizeof(struct Enemy));
                    init_enemy(e1, x, y, id_delta, 1, TRUE);
                    configure_enemy(e1);

                    if (iDropCountdown <= 0) {
                        e1->hasDrop = TRUE;
                        iDropCountdown = 2 + getDropCountdown();
                    } else {
                        iDropCountdown--;

                    }
                    e1->fLifetime = j / 2.0;
                    updatePosition_enemy(e1, e1->iType, e1->iLevel);

                    //					add_node(&listEnemy, e1);
                    iEnemyCount++;
                }

            } else 	if (strLine[i] == '8') {
                //Delta L2
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_echo, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                //				add_node(&listEnemy, e1);
                iEnemyCount++;

            } else 	if (strLine[i] == 'A') {
                //Echo L1
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_foxtrot, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                //				add_node(&listEnemy, e1);
                iEnemyCount++;

            } else 	if (strLine[i] == 'B') {
                //Echo L2
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_foxtrot, 2, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                //				add_node(&listEnemy, e1);
                iEnemyCount++;

            } else 	if (strLine[i] == 'C') {
                //Foxtrot L1
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_golf, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                //				add_node(&listEnemy, e1);
                iEnemyCount++;

            } else 	if (strLine[i] == 'D') {
                //Foxtrot L2
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_golf, 2, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;
            } else 	if (strLine[i] == 'E') {
                //Golf L1
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_hotel, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;

            } else 	if (strLine[i] == 'F') {
                //Golf L2
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_hotel, 2, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;
            } else if (strLine[i] == 'G') {
                //Hotel L1
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_india, 2, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;
            } else if (strLine[i] == 'H') {
                //Hotel L1
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_india, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;
            } else if (strLine[i] == 'I') {
                //Juliett L1
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_juliett, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;
            } else if (strLine[i] == 'J') {
                //Juliett L2
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_juliett, 2, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;
            } else if (strLine[i] == 'K') {
                //Kilo L1
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_kilo, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;

            } else if (strLine[i] == 'L') {
                //Lima L1
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_lima, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;
            } else if (strLine[i] == 'M') {
                //Lima L2
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_lima, 2, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;
            } else if (strLine[i] == 'N') {
                //Lima L2
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_mike, 1, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;
            } else if (strLine[i] == 'O') {
                //Lima L2
                struct Enemy *e1;
                e1 = malloc(sizeof(struct Enemy));
                init_enemy(e1, x, y, id_mike, 2, TRUE);
                configure_enemy(e1);

                if (iDropCountdown <= 0) {
                    e1->hasDrop = TRUE;
                    iDropCountdown = getDropCountdown();
                } else {
                    iDropCountdown--;
                }
                iEnemyCount++;
            }

            x += UNIT_SIZE;

            if (strLine[i] == '\n') {
                x = 0;
                y += UNIT_SIZE;

            }
        }

        if (strLine[i] == '=') {
            iLevel++;
            x = 0;
            y = 0;

        }

    }

    //  printf("Enemies spawned: %d\n", iEnemyCount);

}

int getWorldLevels(int iWorld) {
    int iLevelCount;
    int i;
    int iCols;

    SDL_RWops *f;

    iLevelCount = 0;  //this is really a count of levels after the first one (index 0)
  //  iCols = 64; //only need to read the first character on the line

  /*
      system("pwd");
    printf("Start Reading filea: %s\n", strFile);
      f = SDL_RWFromFile(strFile, "r");
      char strLine[iCols];
      Sint64 iFileSize = SDL_RWsize(f);
      printf("File size: %d\n", iFileSize);
      char strFileContents[iFileSize];
          SDL_RWread( f, strFileContents, sizeof(char), iFileSize );

      printf("*** File Contents: %s\n", strFileContents);

      for (i = 0; i < iFileSize; i++) {
          if (strFileContents[i] == '=') {
              iLevelCount++;

          }
      }
      SDL_RWclose(f);
      */
      //  printf("End Reading file\n");

      //  printf("total levels: %d\n", iLevelCount);

    for (i = 0; i < strlen(strWorldData[iWorld]); i++) {
        if (strWorldData[iWorld][i] == '=') {
            iLevelCount++;
        }

    }

    return iLevelCount;

}

int getDropCountdown() {
    int iReturn;
    iReturn = 5 + (rand() % 3);
    return iReturn;
}
