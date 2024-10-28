#include "NetworkManager.h"
#include <iostream>
#include <string>

int main()
{
  GameObject defaultPlayerPrefab("Player");
  NetworkManager networkManager(defaultPlayerPrefab);

  std::string role;
  std::cout << "Do you want to be a server or a client? ";
  std::cin >> role;

  if (role == "server")
  {
    networkManager.startServer();
  }
  else if (role == "client")
  {
    networkManager.startClient();
  }
  else
  {
    std::cerr << "Invalid input. Please enter 'server' or 'client'." << std::endl;
    return 1;
  }

  return 0;
}