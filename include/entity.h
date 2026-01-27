#ifndef ENTITY_H
#define ENTITY_H



typedef struct
{
    float x, y;
    float vx, vy;
    int w, h;
    float health;
    bool alive;
} Entity;


#endif
