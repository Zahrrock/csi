#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <stdbool.h>
#include "entity.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_HEALTH 100
#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 20
#define PLAYER_SPEED 400.0f

#define BULLET_DAMAGE 200
#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f
#define NB_BULLET_MAX 10

#define ENEMY_HEALTH 100
#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 50
#define ENEMY_SPEED 200.0f
#define NB_ENEMY_BY_ROW 6
#define NB_ENEMY_BY_COLUMN 3
#define NB_ENEMY NB_ENEMY_BY_ROW*NB_ENEMY_BY_COLUMN

#define NB_ENTITY_MAX NB_BULLET_MAX+NB_ENEMY+1

bool init(SDL_Window **window, SDL_Renderer **renderer);
void add_entity(Entity *List_Entities[], Entity *entity);
void remove_entity(Entity *List_Entities[], Entity *entity, Entity *E[]);
void handle_input(bool *running, const Uint8 *keys, Entity *player, bool *new_bullet_demand);

void move(Entity *entity, float dt);
bool collision(Entity *entity1, Entity *entity2);
void update(Entity *player, float dt, Entity *E[], bool *running, bool *new_bullet_demand, Entity *List_Entities[]);

void render(SDL_Renderer *renderer, Entity *List_Entities[]);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);

#endif
