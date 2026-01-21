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

    
    Entity player = {
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - PLAYER_HEIGHT,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .vy = 0,
        .alive = true,
    };
    
    // Tableau contenant les ennemies (E) (ou les Extraterrestres XD )
    Entity E[NB_ENEMY];

    for(int i = 0; i < NB_ENEMY; i++){
        E[i].x = i*(ENEMY_WIDTH + 10);
        E[i].y = 0;
        E[i].w = ENEMY_WIDTH;
        E[i].h = ENEMY_HEIGHT;
        E[i].vx = 0;
        E[i].vy = 100;
        E[i].alive = true;
    }
    
    Entity bullet = {0}; // Initialise tous les attributs de bullet à 0
    bool bullet_active = false;
    
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
        handle_input(&running, keys, &player, &bullet, &bullet_active);
        running = update(&player, &bullet, &bullet_active, dt, E);
        render(renderer, &player, &bullet, bullet_active, E);
    }

    cleanup(window, renderer);
    return 0;
}
