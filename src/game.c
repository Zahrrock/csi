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

void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
    }

    player->vx = 0.0f;
    if (keys[SDL_SCANCODE_LEFT])
        player->vx = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_RIGHT])
        player->vx = PLAYER_SPEED;

    if (keys[SDL_SCANCODE_SPACE] && !*bullet_active)
    {
        *bullet_active = true;
        bullet->x = player->x + player->w / 2 - BULLET_WIDTH / 2;
        bullet->y = player->y;
        bullet->w = BULLET_WIDTH;
        bullet->h = BULLET_HEIGHT;
        bullet->vy = -BULLET_SPEED;
    }
}

bool update(Entity *player, Entity *bullet, bool *bullet_active, float dt, Entity *enemy)
{
    player->x += player->vx * dt;

    if (player->x < 0)
        player->x = 0;
    if (player->x + player->w > SCREEN_WIDTH)
        player->x = SCREEN_WIDTH - player->w;

    if (*bullet_active)
    {
        bullet->y += bullet->vy * dt;
        if (bullet->y + bullet->h < 0)
            *bullet_active = false;
    }
    if (enemy->alive == true){
        // Déplacement de l'enemy
        enemy->y += enemy->vy * dt;
        // Détection collision bullet et enemy
        if (bullet->x + bullet->w > enemy->x && bullet->x < enemy->x + enemy->w && bullet->y < enemy->y + enemy->h){
            enemy->alive = false;
            *bullet_active = false;
            return false;
        }
        // Detection enemy atteint le bas de l'écran 
        if (enemy->y + enemy->h > player->y){
            player->alive = false;
            return false;
        }
    }
    return true;
}

void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, bool bullet_active, Entity *enemy)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // couleur noire (0, 0, 0), opaque (255)
    SDL_RenderClear(renderer); // On efface tout sous un voile noir opaque

    // On definit un rectangle que SDL dessinera. Il contient les coordonnées et le width et height.
    SDL_Rect player_rect = {
        (int)player->x, (int)player->y,
        player->w, player->h};
    if(player->alive == true){
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &player_rect);
    }

    SDL_Rect enemy_rect = {
        (int)enemy->x, (int)enemy->y,
        enemy->w, enemy->h};
    if (enemy->alive == true){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &enemy_rect);
    }

    if (bullet_active)
    {
        SDL_Rect bullet_rect = {
            (int)bullet->x, (int)bullet->y,
            bullet->w, bullet->h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bullet_rect);
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
