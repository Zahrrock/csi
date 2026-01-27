#ifndef ENTITY_H
#define ENTITY_H



typedef struct
{
    float x, y;
    float vx, vy;
    int w, h;
    float health;
    bool alive;
    int type; // PLAYER = 0, ENEMY = 1, BULLET = 2
} Entity;


#endif
