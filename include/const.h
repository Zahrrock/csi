#ifndef CONST_H
#define CONST_H

#include <stdbool.h>

// Nombre d'entités max autorisé pendant le jeu
#define NB_BULLET_PLAYER_MAX 10
#define NB_BULLET_ENEMIES_MAX 10
#define NB_ENEMY_BY_ROW 6
#define NB_ENEMY_BY_COLUMN 3
#define NB_ENEMY NB_ENEMY_BY_ROW*NB_ENEMY_BY_COLUMN
#define NB_ENTITY_MAX NB_BULLET_PLAYER_MAX+NB_BULLET_ENEMIES_MAX+NB_ENEMY+1



#endif