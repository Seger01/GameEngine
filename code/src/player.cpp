#include "player.h"

Player::Player(int x, int y, SDL_Color color)
    : x(x), y(y), color(color), speed(5) {}

void Player::handleInput(SDL_Scancode up, SDL_Scancode down, SDL_Scancode left, SDL_Scancode right)
{
    const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[up])
    {
        y -= speed;
    }
    if (currentKeyStates[down])
    {
        y += speed;
    }
    if (currentKeyStates[left])
    {
        x -= speed;
    }
    if (currentKeyStates[right])
    {
        x += speed;
    }
}

void Player::update()
{
    // Update player state if needed
}

void Player::render(SDL_Renderer *renderer)
{
    SDL_Rect fillRect = {x, y, 50, 50};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
    SDL_RenderFillRect(renderer, &fillRect);
}

int Player::getX() const
{
    return x;
}

int Player::getY() const
{
    return y;
}

void Player::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

SDL_Color Player::getColor() const
{
    return color;
}

void Player::setColor(SDL_Color color)
{
    this->color = color;
}