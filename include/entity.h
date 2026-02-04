#ifndef ENTITY_H
#define ENTITY_H

#include "const.h"

enum Type{
    PLAYER = 0,
    ENEMY_WEAK = 1,
    ENEMY_STRONG = 2,
    BULLET_PLAYER = 3,
    BULLET_ENEMY = 4,
};

typedef struct {
    enum Type type;
    int w, h;
    float x, y, vx, vy, health;

} Entity;

typedef struct {
    bool running;
    int nb_enemies, nb_bullets_player, nb_bullets_enemy, nb_entities;
    Entity *player;
    Entity *Enemies[NB_ENEMY], *Bullets_player[NB_BULLET_PLAYER_MAX], *Bullets_enemies[NB_BULLET_ENEMIES_MAX], *Entities[NB_ENTITY_MAX];

    SDL_Window *window;
    SDL_Renderer *renderer;

    Uint32 last_ticks, new_ticks;
    float current_dt;
} GAME;

#endif
