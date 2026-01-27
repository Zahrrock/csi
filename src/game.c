#include <SDL.h>
#include "const.h"
#include "entity.h"
#include "game.h"
#include <stdio.h>

bool init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Space Invaders (SDL)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!*window)
    {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}
void add_entity(Entity *Storage[], Entity *entity)
{
    for(int i=0;i<NB_ENTITY_MAX;i++){
        if (Storage[i] == NULL){
            Storage[i] = entity;
            break;
        }
    }
}
void remove_entity(Entity *Storage[], Entity *entity)
{
    for(int i=0;i<NB_ENTITY_MAX;i++){
        if (Storage[i] == entity){
            free(entity);
            Storage[i] = NULL;
            break;
        }
    }
}

void handle_input(bool *running, const Uint8 *keys, Entity *player, bool *new_bullet_demand)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_SPACE) {
            *new_bullet_demand = true;
        }
        if (event.type == SDL_QUIT)
            *running = false;
    }

    player->vx = 0.0f;
    if (keys[SDL_SCANCODE_LEFT])
        player->vx = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_RIGHT])
        player->vx = PLAYER_SPEED;
}



void move(Entity *entity, float dt)
{
    entity->x += entity->vx * dt;
    entity->y += entity->vy * dt;
}

bool collision(Entity *entity1, Entity *entity2)
{
    if (entity1->x + entity1->w > entity2->x && entity1->x < entity2->x + entity2->w && entity1->y + entity1->h > entity2->y && entity1->y < entity2->y + entity2->h)
        return true;
    return false;
}

void update(float dt, GAME_STATE Game)
{
    Entity *player = Game.player;
    // DEPLACEMENT des entités
    for(int i=0; i<Game.nb_entity; i++){
        Entity *entity = Game.Entities[i];
        move(entity, dt);
    }
    // BORDERS PLAYER
    if (player->x < 0)
    player->x = 0;
    if (player->x + player->w > SCREEN_WIDTH)
    player->x = SCREEN_WIDTH - player->w;
    
    // UPDATE ENNEMIES
    // Détection atteinte bord droit et gauche écran par les enemy
    float x_min = SCREEN_WIDTH;
    float x_max = 0.0;
    for(int i=0; i<Game.nb_enemy; i++){
        Entity *enemy = Game.E[i];
        if (enemy->alive == true){
            if (enemy->x < x_min)
            x_min = enemy->x;
            if (enemy->x + enemy->w > x_max)
            x_max = enemy->x+enemy->w;
        }
    }
    if (x_min < 0){
        for(int i=0; i<Game.nb_enemy; i++){
            Entity *enemy = Game.E[i];
            enemy->vx = ENEMY_SPEED;
        } 
    }
    if (x_max > SCREEN_WIDTH){
        for(int i=0; i<Game.nb_enemy; i++){
            Entity *enemy = Game.E[i];
            enemy->vx = -ENEMY_SPEED;
        } 
    }
    
    // Détection d'atteinte du bas de l'écran des enemy
    for(int i=0; i<Game.nb_enemy; i++){
        Entity *enemy = Game.E[i];
        if (enemy->y + enemy->h > player->y){
            player->health = 0;
        }
    }
    
    // tir des enemy
    //srand(time(NULL));  // initialise le générateur
    //int r = rand() % NB_ENEMY;
    
    // DETECTION des collisions
    for(int i=0; i<Game.nb_bullets; i++){
        Entity *bullet = Game.B[i];
        for(int j=0; j<Game.nb_enemy; j++){
            Entity *enemy = Game.E[i];
            if (collision(bullet, enemy)){
                bullet->health = 0;
                enemy->health = 0;
            }
        }
    }
}



void render(SDL_Renderer *renderer, GAME_STATE Game)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // couleur noire (0, 0, 0), opaque (255)
    SDL_RenderClear(renderer); // On efface tout sous un voile noir opaque

    for(int i=0; i<NB_ENTITY_MAX; i++){
        Entity *entity = Game.Entities[i];
        if (entity != NULL && entity->alive == true){
            // On definit un rectangle que SDL dessinera. Il contient les coordonnées, le width et height.
            SDL_Rect rect = {
                (int)entity->x, (int)entity->y,
                entity->w, entity->h
            };
            if (entity->type == 0)
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            if (entity->type == 1)
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            if (entity->type == 2)
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}
