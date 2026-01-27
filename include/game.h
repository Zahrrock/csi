#ifndef GAME_H
#define GAME_H

bool init(SDL_Window **window, SDL_Renderer **renderer);
void add_entity(Entity *Storage[], Entity *entity);
void remove_entity(Entity *Storage[], Entity *entity);
void handle_input(bool *running, const Uint8 *keys, Entity *player, bool *new_bullet_demand);

void move(Entity *entity, float dt);
bool collision(Entity *entity1, Entity *entity2);
void update(float dt, GAME_STATE Game);

void render(SDL_Renderer *renderer, GAME_STATE Game);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);

#endif
