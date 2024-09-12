#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class Player
{
public:
    Player(int x, int y, SDL_Color color);
    void handleInput(bool up, bool down, bool left, bool right);
    void update();
    void render(SDL_Renderer *renderer);
    int getX() const;
    int getY() const;
    void setPosition(int x, int y);
    SDL_Color getColor() const;
    void setColor(SDL_Color color);

private:
    int x, y;
    SDL_Color color;
    int speed;
};

#endif // PLAYER_H