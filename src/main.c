#include "entity.h"
#include "game.h"


int main(){
    // INITIALISATION
    GAME Game;
    if (init(&Game) == 1)
        return 1;
    
    // BOUCLE DE JEU
    while(Game.running){
        update(&Game);
    }

    cleanup(Game.window, Game.renderer);
    return 0;
}