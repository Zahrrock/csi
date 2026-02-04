#include "entity.h"
#include "game.h"


int main(){
    // INITIALISATION
    GAME Game;
    if (init(&Game) == 1)
        return 1;
    
    // BOUCLE DE JEU
    int i = 0;
    while(i<1000){
        update(&Game);
        i++;
    }

    cleanup(Game.window, Game.renderer);
    return 0;
}