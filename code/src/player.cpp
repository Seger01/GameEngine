#include "player.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 50;

// Constructor to initialize player position, color, and speed
Player::Player(int x, int y, SDL_Color color)
    : x(x), y(y), color(color), speed(5) {}

// Handle player input for movement
void Player::handleInput(bool up, bool down, bool left, bool right)
{
    if (up)
    {
        y -= speed;
    }
    if (down)
    {
        y += speed;
    }
    if (left)
    {
        x -= speed;
    }
    if (right)
    {
        x += speed;
    }
}

// Update player position and ensure it stays within bounds
void Player::update()
{
    if (x < 0)
    {
        x = 0;
    }
    if (x + BLOCK_SIZE > SCREEN_WIDTH)
    {
        x = SCREEN_WIDTH - BLOCK_SIZE;
    }
    if (y < 0)
    {
        y = 0;
    }
    if (y + BLOCK_SIZE > SCREEN_HEIGHT)
    {
        y = SCREEN_HEIGHT - BLOCK_SIZE;
    }
}

// Render the player on the screen
void Player::render(SDL_Renderer *renderer)
{
    SDL_Rect fillRect = {x, y, BLOCK_SIZE, BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
    SDL_RenderFillRect(renderer, &fillRect);
}

// Get the player's X position
int Player::getX() const
{
    return x;
}

// Get the player's Y position
int Player::getY() const
{
    return y;
}

// Set the player's position
void Player::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

// Get the player's color
SDL_Color Player::getColor() const
{
    return color;
}

// Set the player's color
void Player::setColor(SDL_Color color)
{
    this->color = color;
}