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
            Game->Enemies[j*NB_ENEMY_BY_ROW+i] = malloc(sizeof(Entity));
            *(Game->Enemies[j*NB_ENEMY_BY_ROW+i]) = (Entity){
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
void append(GAME *Game, Entity *List[], Entity *entity)
{
    int i_max=0;
    if(List == Game->Enemies){
        i_max = NB_ENEMY;
    }
    if(List == Game->Bullets_enemies){
        i_max = NB_BULLET_ENEMIES_MAX;
    }
    if(List == Game->Bullets_player){
        i_max = NB_BULLET_PLAYER_MAX;
    }
    if(List == Game->Entities){
        i_max = NB_ENTITY_MAX;
    }
    
    if(entity == NULL)
        printf("Attention, ajout d'un NULL \n");
    for(int i=0;i<i_max;i++){
        if (List[i] == NULL){
            List[i] = entity;
            
            break;
        }
    }
}
void delete(GAME *Game, Entity *entity)
{
    Entity **List;
    int i_max=0;
    if(entity->type == ENEMY_WEAK || entity->type == ENEMY_STRONG){
        List = Game->Enemies;
        i_max = Game->nb_enemies;
    }
    if(entity->type == BULLET_PLAYER){
        List = Game->Bullets_player;
        i_max = Game->nb_bullets_player;
    }
    if(entity->type == BULLET_ENEMY){
        List = Game->Bullets_enemies;
        i_max = Game->nb_bullets_enemy;
    }
    int indice=0;
    for(int i=0;i<i_max;i++){
        if (List[i] == entity){
            indice = i;
            free(entity);
            List[i] = NULL;
            break;
        }
    }
    for(int i=indice;i<i_max-1;i++){
        List[i] = List[i+1];
    }
    List[i_max-1] = NULL;
}
void new_bullet(GAME *Game){
    Entity *bullet = malloc(sizeof(Entity));
    *bullet = (Entity){
        .type = BULLET_PLAYER,
        .x = Game->player->x + Game->player->w / 2 - BULLET_WIDTH / 2,
        .y = Game->player->y,
        .w = BULLET_WIDTH,
        .h = BULLET_HEIGHT,
        .vx = 0,
        .vy = -BULLET_SPEED,
        .health = PLAYER_HEALTH,
    };
    append(Game, Game->Bullets_player, bullet);
    Game->nb_bullets_player++;
}
void handle_input(GAME *Game)
{
    SDL_PumpEvents();
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_SPACE) {
            new_bullet(Game);
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
void entities_inventory(GAME *Game)
{
    for(int i=0;i<NB_ENTITY_MAX;i++){
        Game->Entities[i] = NULL;
    }

    append(Game, Game->Entities, Game->player);
    Game->nb_entities = 1;
    for(int i=0;i<Game->nb_enemies;i++){
        append(Game, Game->Entities, Game->Enemies[i]);
        Game->nb_entities++;
    }
    for(int i=0;i<Game->nb_bullets_player;i++){
        append(Game, Game->Entities, Game->Bullets_player[i]);
        Game->nb_entities++;
    }
    for(int i=0;i<Game->nb_bullets_enemy;i++){
        append(Game, Game->Entities, Game->Bullets_enemies[i]);
        Game->nb_entities++;
    }
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
bool collision(Entity *entity1, Entity *entity2)
{
    if (entity1->x + entity1->w >= entity2->x && entity1->x <= entity2->x + entity2->w && entity1->y + entity1->h >= entity2->y && entity1->y <= entity2->y + entity2->h)
        return true;
    return false;
}
void check_borders(GAME *Game){
    // ENEMY ATTEINT BORD DROIT OU GAUCHE
    float x_min = SCREEN_WIDTH;
    float x_max = 0.0;
    float y_max = 0.0;
    for(int i=0; i<Game->nb_enemies; i++){
        Entity *enemy = Game->Enemies[i];
        if (enemy->x < x_min)
            x_min = enemy->x;
        if (enemy->x + enemy->w > x_max)
            x_max = enemy->x+enemy->w;
        if(enemy->y+ENEMY_HEIGHT> y_max)
            y_max = enemy->y+ENEMY_HEIGHT;
    }
    if (x_min < Game->current_dt*ENEMY_SPEED){
        for(int i=0; i<Game->nb_enemies; i++){
            Game->Enemies[i]->vx = ENEMY_SPEED;
        } 
    }
    if (x_max > SCREEN_WIDTH - Game->current_dt*ENEMY_SPEED){
        for(int i=0; i<Game->nb_enemies; i++){
            Game->Enemies[i]->vx = -ENEMY_SPEED;
        } 
    }

    // ENEMY ATTEINT BAS
    if(y_max > SCREEN_HEIGHT - PLAYER_HEIGHT)
        Game->running = false;

}
void update_positions(GAME *Game)
{
    check_borders(Game);
    for(int i=0;i<Game->nb_entities;i++){
        Entity *entity = Game->Entities[i];
        float new_x = entity->x + entity->vx * Game->current_dt;
        float new_y = entity->y + entity->vy * Game->current_dt;
        bool is_collision = false;
        
        if(entity->type == PLAYER){
            if(new_x>SCREEN_WIDTH)
                new_x = SCREEN_WIDTH-PLAYER_WIDTH;
            if(new_x<0)
                new_x = 0;
            for(int i=0;i<Game->nb_bullets_enemy;i++){
                if(collision(entity, Game->Bullets_enemies[i])){
                    Game->player->health = 0;
                    delete(Game, Game->Bullets_enemies[i]);
                    Game->nb_bullets_enemy--;
                    Game->running = false;
                    is_collision = true;
                    break;
                }

            }

        }
        if(entity->type == ENEMY_WEAK ||entity->type == ENEMY_STRONG){
            for(int i=0;i<Game->nb_bullets_player;i++){
                if(collision(entity, Game->Bullets_player[i])){
                    delete(Game, Game->Bullets_player[i]);
                    Game->nb_bullets_player--;
                    delete(Game, entity);
                    Game->nb_enemies--;
                    is_collision = true;
                    break;
                }

            }

        }
        if(entity->type == BULLET_PLAYER){
            for(int i=0;i<Game->nb_enemies;i++){
                if(collision(entity, Game->Enemies[i])){
                    delete(Game, Game->Enemies[i]);
                    Game->nb_enemies--;
                    delete(Game, entity);
                    Game->nb_bullets_player--;
                    is_collision = true;
                    break;
                }

            }

        }
        if(entity->type == BULLET_ENEMY){
            if(collision(entity, Game->player)){
                delete(Game, entity);
                Game->nb_bullets_enemy--;
                Game->player->health = 0;
                Game->running = false;
                is_collision = true;
                break;
            }
        }
        
        if(is_collision){
            entities_inventory(Game);
            continue;
        }
        else{
            entity->x = new_x;
            entity->y = new_y;
        }
    }
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