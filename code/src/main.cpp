#include "game.h"
#include <iostream>
#include <cstring>

int main(int argc, char *args[])
{
  Game game;

  if (!game.init())
  {
    std::cerr << "Failed to initialize the game." << std::endl;
    return -1;
  }

  game.run();
  game.cleanup();

  return 0;
}