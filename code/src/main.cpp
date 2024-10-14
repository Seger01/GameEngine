// #include "server.h"
// #include "client.h"
// #include <iostream>
// #include <cstring>

// int main(int argc, char *args[])
// {
// if (argc > 1 && strcmp(args[1], "server") == 0)
// {
//   Server server;
//   server.init();
//   server.run();
//   server.cleanup();
// }
// else
// {
//   Client client;
//   client.init();
//   client.run();
//   client.cleanup();
// }

// }

#include "slikenet/peerinterface.h"
#include "NetworkManager.h"
#include "NetworkObject.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include <iostream>
#include <SDL2/SDL.h>

// Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BLOCK_SIZE = 50;

class blockObject : public NetworkObject
{
public:
  blockObject() : NetworkObject(), mX(0), mY(0) {}

private:
  float mX, mY;
};

int SDLInit(SDL_Window *window, SDL_Renderer *renderer, std::string title)
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  // Create SDL window
  window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL)
  {
    std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  // Create SDL renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL)
  {
    std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    return -1;
  }
  return 0;
}

int main(int argc, char *args[])
{
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  // Initialize network components
  NetworkManager manager;
  NetworkClient client;
  NetworkServer server;

  // Get mode input from user
  int modeInput;
  NetworkRole mode;
  std::cout << "Enter mode: 1 for Server, 2 for Client, 3 for Host: ";
  std::cin >> modeInput;

  // Set mode based on user input
  switch (modeInput)
  {
  case 1:
    mode = SERVER;
    break;
  case 2:
    mode = CLIENT;
    break;
  case 3:
    mode = HOST;
    break;
  default:
    std::cerr << "Invalid mode selected!" << std::endl;
    return -1;
  }

  // Start server or host, or connect client
  if (mode == SERVER)
  {
    manager.StartServer(60000);
    server.gameObjects.push_back(blockObject());
  }
  else if (mode == CLIENT)
  {
    SDLInit(window, renderer, "blockGame Client");
    client.Connect(manager, "127.0.0.1", 60000);
  }
  else if (mode == HOST)
  {
    SDLInit(window, renderer, "blockGame Host");
    manager.StartHost(60000);
    server.gameObjects.push_back(blockObject());
  }

  bool quit = false;
  SDL_Event e;
  Uint32 frameDelay = 1000 / 60;

  // Main game loop
  while (!quit)
  {
    Uint32 frameStart = SDL_GetTicks();

    // Handle events
    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_QUIT)
      {
        quit = true;
      }
      else if (e.type == SDL_KEYDOWN)
      {
        float dx = 0, dy = 0;
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
          dy = -1;
          break;
        case SDLK_DOWN:
          dy = 1;
          break;
        case SDLK_LEFT:
          dx = -1;
          break;
        case SDLK_RIGHT:
          dx = 1;
          break;
        }
        // Send input to server or host
        if (mode == CLIENT || mode == HOST)
        {
          client.SendInput(manager.mPeer, dx, dy);
        }
        // Move object on server or host
        if (mode == SERVER || mode == HOST)
        {
          server.gameObjects[0].MoveObject(server.gameObjects[0], dx, dy);
        }
      }
    }

    // Sync objects on server or host
    if (mode == SERVER || mode == HOST)
    {
      server.SyncObjects(manager.mPeer);
    }

    // Update network manager
    manager.Update();

    // Render objects on client or host
    if (mode == CLIENT || mode == HOST)
    {
      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
      SDL_RenderClear(renderer);

      for (auto &obj : server.gameObjects)
      {
        SDL_Rect fillRect = {static_cast<int>(obj.mX), static_cast<int>(obj.mY), BLOCK_SIZE, BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0xFF);
        SDL_RenderFillRect(renderer, &fillRect);
      }

      SDL_RenderPresent(renderer);
    }

    // Frame delay to cap frame rate
    int frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
    {
      SDL_Delay(frameDelay - frameTime);
    }
  }

  // Cleanup SDL
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}