#include <stdio.h>
#include "entity.h"


bool init_SDL(SDL_Window **window, SDL_Renderer **renderer)
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
int init(GAME *Game){
    // INITIALISATION DE GAME
    *Game = (GAME){
        .running = true,
        .nb_enemies = NB_ENEMY,
        .nb_bullets_player = 0,
        .nb_bullets_enemy = 0,
        .nb_entities = NB_ENEMY + 1,
        .player = NULL,
        .Enemies = {NULL},
        .Bullets_enemies = {NULL},
        .Bullets_player = {NULL},
        .Entities = {NULL},
        .window = NULL,
        .renderer = NULL,
    };

    Game->player = malloc(sizeof(Entity));
    *(Game->player) = (Entity){
        .type = PLAYER,
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - PLAYER_HEIGHT,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .vy = 0,
        .health = PLAYER_HEALTH,
    };

    for(int i = 0; i < NB_ENEMY_BY_ROW; i++){
        for(int j=0; j<NB_ENEMY_BY_COLUMN; j++){
            Game->Enemies[i*NB_ENEMY_BY_COLUMN+j] = malloc(sizeof(Entity));
            *(Game->Enemies[i*NB_ENEMY_BY_COLUMN+j]) = (Entity){
                .type = ENEMY_WEAK,
                .x = i*(ENEMY_WIDTH + 10),
                .y = j*(ENEMY_HEIGHT + 10),
                .w = ENEMY_WIDTH,
                .h = ENEMY_HEIGHT,
                .vx = ENEMY_SPEED,
                .vy = ENEMY_SPEED/10,
                .health = ENEMY_HEALTH,
        
            };
        }
    }

    // INITIALISATION DE SDL
    if (!init_SDL(&(Game->window), &(Game->renderer)))
        return 1;
    
   return 0;
}
void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}
void handle_input(GAME *Game)
{
    SDL_PumpEvents();
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_SPACE) {
            Game->new_bullet_demand= true;
        }
        if (event.type == SDL_QUIT)
            Game->running = false;
    }

    (Game->player)->vx = 0.0f;
    if (keys[SDL_SCANCODE_LEFT])
        (Game->player)->vx = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_RIGHT])
        (Game->player)->vx = PLAYER_SPEED;
}
void update_time(GAME *Game)
{
    Game->new_ticks = SDL_GetTicks();
    Game->current_dt = (Game->new_ticks - Game->last_ticks) / 1000.0f;
    if (Game->current_dt > 0.05f)
        Game->current_dt = 0.05f;
    Game->last_ticks = Game->new_ticks;
}
void append(Entity *Entities[], Entity *entity)
{
    if(entity == NULL)
        printf("Attention, ajout d'un NULL \n");
    for(int i=0;i<NB_ENTITY_MAX;i++){
        if (Entities[i] == NULL){
            Entities[i] = entity;
            break;
        }
    }
}
void entities_inventory(GAME *Game)
{
    for(int i=0;i<NB_ENTITY_MAX;i++){
        Game->Entities[i] = NULL;
    }

    append(Game->Entities, Game->player);
    Game->nb_entities = 1;
    for(int i=0;i<Game->nb_enemies;i++){
        append(Game->Entities, Game->Enemies[i]);
        Game->nb_entities++;
    }
    for(int i=0;i<Game->nb_bullets_player;i++){
        append(Game->Entities, Game->Bullets_player[i]);
        Game->nb_entities++;
    }
    for(int i=0;i<Game->nb_bullets_enemy;i++){
        append(Game->Entities, Game->Bullets_enemies[i]);
        Game->nb_entities++;
    }
    printf("%i", Game->nb_entities);
}
void display(GAME *Game)
{
    // ON EFFACE TOUT
    SDL_SetRenderDrawColor(Game->renderer, 0, 0, 0, 255); // couleur noire (0, 0, 0), opaque (255)
    SDL_RenderClear(Game->renderer); // On efface tout sous un voile noir opaque

    // ON REDESSINE TOUTES LES ENTITES
    for(int i=0; i<Game->nb_entities; i++){
        Entity *entity = Game->Entities[i];
        if(entity == NULL)
            printf("Attention, affichage d'un NULL à Entities[i] et i = %i", i);
        // On definit un rectangle que SDL dessinera. Il contient les coordonnées, le width et height.
        SDL_Rect rect = {
            (int)entity->x, (int)entity->y,
            entity->w, entity->h
        };
        if (entity->type == PLAYER)
            SDL_SetRenderDrawColor(Game->renderer, 0, 255, 0, 255);
        if (entity->type == ENEMY_WEAK || entity->type == ENEMY_STRONG)
            SDL_SetRenderDrawColor(Game->renderer, 255, 0, 0, 255);
        if (entity->type == BULLET_PLAYER || entity->type == BULLET_ENEMY)
            SDL_SetRenderDrawColor(Game->renderer, 255, 255, 255, 255);

        SDL_RenderFillRect(Game->renderer, &rect);
    }

    SDL_RenderPresent(Game->renderer);
}
void update_positions(GAME *Game)
{
    
}

void update(GAME* Game)
{
    update_time(Game);
    handle_input(Game);
    entities_inventory(Game);
    update_positions(Game);
    entities_inventory(Game);
    display(Game);
}