#include <SDL.h>
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
void add_entity(Entity *List_Entities[], Entity *entity)
{
    for(int i=0;i<NB_ENTITY_MAX;i++){
        if (List_Entities[i] == NULL){
            List_Entities[i] = entity;
            break;
        }
    }
}
void remove_entity(Entity *List_Entities[], Entity *entity, Entity *E[])
{
    for(int i=0;i<NB_ENTITY_MAX;i++){
        if (List_Entities[i] == entity){
            free(entity);
            List_Entities[i] = NULL;
            break;
        }
    }
    for(int i=0;i<NB_ENTITY_MAX;i++){
        if (E[i] == entity){
            E[i] = NULL;
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

void update(Entity *player, float dt, Entity *E[], bool *running, bool *new_bullet_demand, Entity *List_Entities[])
{
    // DEPLACEMENT des entités
    for(int i=0; i<NB_ENTITY_MAX; i++){
        Entity *entity = List_Entities[i];
        if (entity != NULL)
            move(entity, dt);
    }
    // DETECTION des collisions
    for(int i=0; i<NB_ENTITY_MAX; i++){
        Entity *entity1 = List_Entities[i];
        if (entity1 != NULL && entity1->type == 2){
            for(int i=0; i<NB_ENTITY_MAX; i++){
                Entity *entity2 = List_Entities[i];
                if (entity2 != NULL && entity2->type != 2){
                    if (collision(entity1, entity2)){
                        entity2->health = 0;
                        entity1->health = 0;
                    }
                }
            }
        }
    }
    // BORDERS PLAYER
    if (player->x < 0)
        player->x = 0;
    if (player->x + player->w > SCREEN_WIDTH)
        player->x = SCREEN_WIDTH - player->w;

    // NEW BULLET
    if (*new_bullet_demand) {
        Entity *bullet = malloc(sizeof(Entity));
        *bullet = (Entity){
            .x = player->x + player->w / 2 - BULLET_WIDTH / 2,
            .y = player->y - BULLET_HEIGHT - 10,
            .w = BULLET_WIDTH,
            .h = BULLET_HEIGHT,
            .vx = 0,
            .vy = -BULLET_SPEED,
            .health = PLAYER_HEALTH,
            .alive = true,
            .type = 2,
        };

        add_entity(List_Entities, bullet);
        *new_bullet_demand = false;
    }

    int nb_enemy_alive = 0;

    // UPDATE ENNEMIES
    // Détection atteinte bord droit et gauche écran par les enemy
    float x_min = SCREEN_WIDTH;
    float x_max = 0.0;
    for(int i=0; i<NB_ENEMY; i++){
        if (E[i] != NULL){
            Entity *enemy = E[i];
            if (enemy->alive == true){
                if (enemy->x < x_min)
                    x_min = enemy->x;
                if (enemy->x + enemy->w > x_max)
                    x_max = enemy->x+enemy->w;
            }
        }
    }
    if (x_min < 0){
        for(int i=0; i<NB_ENEMY; i++){
            if (E[i] != NULL){
                Entity *enemy = E[i];
                enemy->vx = ENEMY_SPEED;
            }
        } 
    }
    if (x_max > SCREEN_WIDTH){
        for(int i=0; i<NB_ENEMY; i++){
            if (E[i] != NULL){
                Entity *enemy = E[i];
                enemy->vx = -ENEMY_SPEED;
            }
        } 
    }

    // Détection d'atteinte du bas de l'écran des enemy
    for(int i=0; i<NB_ENEMY; i++){
        if (E[i] != NULL){
            Entity *enemy = E[i];
            if (enemy->alive == true){
                nb_enemy_alive++;
                // Detection enemy atteint le bas de l'écran 
                if (enemy->y + enemy->h > player->y){
                    player->health = 0;
                    *running = false;
                }
            }
        }
    }

    // tir des enemy
    //srand(time(NULL));  // initialise le générateur
    //int r = rand() % NB_ENEMY;
    
    // SUPPRESSION DES ENTITES MORTES
    for(int i=0; i<NB_ENTITY_MAX; i++){
        Entity *entity = List_Entities[i];
        if (entity != NULL && entity->alive == true && entity->health <= 0){
            entity->alive = false;
            remove_entity(List_Entities, entity, E);
        }
    }
    if (nb_enemy_alive == 0 || player->health == 0)
        *running = false;
}



void render(SDL_Renderer *renderer, Entity *List_Entities[])
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // couleur noire (0, 0, 0), opaque (255)
    SDL_RenderClear(renderer); // On efface tout sous un voile noir opaque

    for(int i=0; i<NB_ENTITY_MAX; i++){
        Entity *entity = List_Entities[i];
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
