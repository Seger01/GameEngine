#include "game.h"
#include <iostream>
#include "player.h"
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 50;

Game::Game()
    : window(NULL), renderer(NULL), quit(false), frameDelay(1000 / 60)
{ // Seed the random number generator
    std::srand(std::time(0));
    // int red = std::rand() % 256;
    // int green = std::rand() % 256;
    // int blue = std::rand() % 256;
}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("blockGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Generate random color for the player
    Uint8 r = rand() % 256;
    Uint8 g = rand() % 256;
    Uint8 b = rand() % 256;

    // Add a player
    players.push_back(Player(100, 100, {r, g, b}));

    return true;
}

void Game::run()
{
    while (!quit)
    {
        Uint32 frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        int frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::cleanup()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
    }

    // Handle input for the player
    players[0].handleInput(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT);
}

void Game::update()
{
    for (auto &player : players)
    {
        player.update();
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    for (auto &player : players)
    {
        player.render(renderer);
    }

    SDL_RenderPresent(renderer);
}