#include <SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "game.h"

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (!init(&window, &renderer))
    {
        return 1;
    }

    // CREATION STORAGE
    Entity *Storage[NB_ENTITY_MAX];
    for(int i=0; i<NB_ENTITY_MAX; i++){
        Storage[i] = NULL;
    }
    // AJOUT PLAYER
    Entity *player = malloc(sizeof(Entity));
    *player = (Entity){
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - PLAYER_HEIGHT,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .vy = 0,
        .health = PLAYER_HEALTH,
        .alive = true,
        .type = 0,
    };
    add_entity(Storage, player);
    // AJOUT ENEMIES
    for(int i = 0; i < NB_ENEMY_BY_ROW; i++){
        for(int j=0; j<NB_ENEMY_BY_COLUMN; j++){
            Entity *enemy = malloc(sizeof(Entity));
            *enemy = (Entity){
                .x = i*(ENEMY_WIDTH + 10),
                .y = j*(ENEMY_HEIGHT + 10),
                .w = ENEMY_WIDTH,
                .h = ENEMY_HEIGHT,
                .vx = ENEMY_SPEED,
                .vy = ENEMY_SPEED/10,
                .health = ENEMY_HEALTH,
                .alive = true,
                .type = 1,
        
            };
            add_entity(Storage, enemy);
        }
    }
    
    bool new_bullet_demand = false;

    bool running = true;
    Uint32 last_ticks = SDL_GetTicks();
    while (running)
    {
        // PRISE DU TEMPS
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f)
            dt = 0.05f;
        last_ticks = ticks;

        // CHARGEMENT DONNEES SDL
        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, player, &new_bullet_demand);

        // NEW GAME STATE
        GAME_STATE Game = {
            .nb_bullets = 0,
            .nb_enemy = 0,
            .nb_entity = 0,
        };
        for(int i=0;i<NB_ENTITY_MAX;i++){
            Entity *entity = Storage[i];
            if (entity != NULL && entity->alive == true){
                Game.Entities[Game.nb_entity] = entity;
                Game.nb_entity++;
                if (entity->type == 0)
                    Game.player = entity;
                if (entity->type == 1){
                    Game.E[Game.nb_enemy] = entity; 
                    Game.nb_enemy++;
                }
                if (entity->type == 2){
                    Game.B[Game.nb_bullets] = entity; 
                    Game.nb_bullets++;    
                }
            }
        }
        // NEW BULLET
        if (new_bullet_demand) {
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
            
            add_entity(Storage, bullet);
            new_bullet_demand = false;
        }
        update(dt, Game);
        render(renderer, Game);
        // SUPPRESSION DES ENTITES MORTES
        for(int i=0; i<Game.nb_entity; i++){
            Entity *entity = Game.Entities[i];
            if (entity->health <= 0){
                entity->alive = false;
                remove_entity(Storage, entity);
            }
        }
        // CONDITION FIN DE JEU
        if (Game.nb_enemy == 0 || player->alive == false)
            running = false;
    }

    cleanup(window, renderer);
    return 0;
}
