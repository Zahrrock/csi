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
    Entity *List_Entities[NB_ENTITY_MAX]; // en première position on a le joueur, puis on a NB_ENTITY_MAX emplacements d'enemy, puis NB_BULLET_MAX emplacements de bullet
    for(int i=0; i<NB_ENTITY_MAX; i++){
        List_Entities[i] = NULL;
    }

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
    add_entity(List_Entities, player);
    // Tableau contenant les pointeurs des ennemies (E) (ou les Extraterrestres XD )
    Entity *E[NB_ENEMY];
    // On initialise les coordonnées de tous les enemy
    for(int i = 0; i < NB_ENEMY_BY_ROW; i++){
        for(int j=0; j<NB_ENEMY_BY_COLUMN; j++){
            E[i*3+j] = malloc(sizeof(Entity));
            *E[i*3+j] = (Entity){
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
            add_entity(List_Entities, E[i*3+j]);
        }
    }
    
    bool new_bullet_demand = false;

    bool running = true;
    Uint32 last_ticks = SDL_GetTicks();
    while (running)
    {
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f)
            dt = 0.05f;
        last_ticks = ticks;

        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, player, &new_bullet_demand);
        update(player, dt, E, &running, &new_bullet_demand, List_Entities);
        render(renderer, List_Entities);
    }

    cleanup(window, renderer);
    return 0;
}
