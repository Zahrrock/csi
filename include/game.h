#ifndef GAME_H
#define GAME_H

int init(GAME *Game);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void update(GAME* Game);

#endif
