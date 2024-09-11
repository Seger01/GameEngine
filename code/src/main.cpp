#include "game.h"
#include <iostream>
#include <cstring>

int main(int argc, char *args[])
{
  Game game;

  // Check command-line arguments
  if (argc > 1 && std::strcmp(args[1], "server") == 0)
  {
    game.isServer = true;
  }
  else
  {
    game.isServer = false;
  }

  if (!game.init())
  {
    std::cerr << "Failed to initialize the game." << std::endl;
    return -1;
  }

  game.run();
  game.cleanup();

  return 0;
}