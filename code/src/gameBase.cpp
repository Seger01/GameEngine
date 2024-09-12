#include "gameBase.h"
#include "player.h"

#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()
#include <iostream>

#include <slikenet/sleep.h>
#include <slikenet/BitStream.h>
#include <slikenet/MessageIdentifiers.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 50;

GameBase::GameBase(SLNet::RakPeerInterface *peer)
    : window(NULL), renderer(NULL), quit(false), frameDelay(1000 / 60), playerID(-1)
{
    this->peer = peer;
}

bool GameBase::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("blockGame Server", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

    return true;
}

void GameBase::run()
{
    while (!quit)
    {
        Uint32 frameStart = SDL_GetTicks();

        handleEvents();
        handleNetwork();
        update();
        render();

        int frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void GameBase::cleanup()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GameBase::update()
{
    for (auto &player : players)
    {
        player.update();
    }
}

void GameBase::render()
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Rect fillRect = {1, 1, 3, 3};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0xFF);
    SDL_RenderFillRect(renderer, &fillRect);

    for (auto &player : players)
    {
        player.render(renderer);
    }

    SDL_RenderPresent(renderer);
}