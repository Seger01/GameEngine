#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class Player
{
public:
    Player(int startX, int startY, SDL_Color playerColor);

    void handleInput(SDL_Scancode up, SDL_Scancode down, SDL_Scancode left, SDL_Scancode right);
    void update();
    void render(SDL_Renderer *renderer);

private:
    int x, y;
    int velX, velY;
    SDL_Color color;
};

#endif // PLAYER_H