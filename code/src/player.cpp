#include "player.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 50;

Player::Player(int startX, int startY, SDL_Color playerColor)
    : x(startX), y(startY), velX(0), velY(0), color(playerColor) {}

void Player::handleInput(SDL_Scancode up, SDL_Scancode down, SDL_Scancode left, SDL_Scancode right)
{
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    velX = velY = 0;

    if (keyState[up])
    {
        velY = -2;
    }
    if (keyState[down])
    {
        velY = 2;
    }
    if (keyState[left])
    {
        velX = -2;
    }
    if (keyState[right])
    {
        velX = 2;
    }
}

void Player::update()
{
    x += velX;
    y += velY;

    // Boundary check
    if (x < 0)
        x = 0;
    if (x + BLOCK_SIZE > SCREEN_WIDTH)
        x = SCREEN_WIDTH - BLOCK_SIZE;
    if (y < 0)
        y = 0;
    if (y + BLOCK_SIZE > SCREEN_HEIGHT)
        y = SCREEN_HEIGHT - BLOCK_SIZE;
}

void Player::render(SDL_Renderer *renderer)
{
    SDL_Rect fillRect = {x, y, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &fillRect);
}