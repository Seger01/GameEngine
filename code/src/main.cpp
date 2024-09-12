#include "server.h"
#include "client.h"
#include <iostream>
#include <cstring>

int main(int argc, char *args[])
{

  if (argc > 1 && strcmp(args[1], "server") == 0)
  {
    Server server;
    server.init();
    server.run();
    server.cleanup();
  }
  else
  {
    Client client;
    client.init();
    client.run();
    client.cleanup();
  }

  return 0;
}