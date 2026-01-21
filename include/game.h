#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <stdbool.h>
#include "entity.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 20
#define PLAYER_SPEED 400.0f

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f

#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 50
#define ENEMY_SPEED 200.0f
#define NB_ENEMY 10

bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active);
bool update(Entity *player, Entity *bullet, bool *bullet_active, float dt, Entity E[]);
void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, bool bullet_active, Entity E[]);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);

#endif
