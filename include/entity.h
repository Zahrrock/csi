#ifndef ENTITY_H
#define ENTITY_H
#include "const.h"

typedef struct
{
    float x, y;
    float vx, vy;
    int w, h;
    float health;
    bool alive;
    int type; // PLAYER = 0, ENEMY = 1, BULLET = 2
} Entity;

typedef struct 
{
    Entity *player;
    Entity *E[NB_ENEMY];
    Entity *B[NB_BULLET_MAX];
    Entity *Entities[NB_ENTITY_MAX];
    int nb_entity;
    int nb_enemy;
    int nb_bullets;
} GAME_STATE;

#endif
