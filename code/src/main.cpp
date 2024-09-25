#include <iostream>

#include "test.h"
#include "GameObject.h"

int main() {
  GameObject player;
  GameObject enemy;
  enemy.setName("enemyObject");
  player.setName("playerObject");

  if (player.addComponent("RenderComponent"))
  {
    std::cout << "Component added successfully\n";
  }
  else 
  {
    std::cout << "Adding component failed\n";
  }

  if (player.addComponent("PhysicsComponent"))
  {
    std::cout << "Component added successfully\n";
  }
  else 
  {
    std::cout << "Adding component failed\n";
  }

  double deltaTime = 0.016;
  player.Update(deltaTime);

  RenderComponent* renderComponent = player.getRenderComponent();
  if(renderComponent)
  {
    renderComponent->setTexturePath("empty.png");
    std::cout << "Render Texture Path: " << renderComponent->getTexturePath() << "\n";
  
    renderComponent->setOpacity(0.5);
    std::cout << "Render opacity Texture Path: " << renderComponent->getOpacity() << "\n";
  }

  PhysicsComponent* physicsComponent = player.getPhysicsComponent();
  if (physicsComponent)
  {
      physicsComponent->setMass(80.0);
      std::cout << "Physics Mass: " << physicsComponent->getMass() << "\n";

      physicsComponent->setFriction(0.5);
      std::cout << "Physics Friction: " << physicsComponent->getFriction() << "\n";
  }
  

  std::cout << "GameObject Name: " << player.getName() << "\n";
  std::cout << "GameObject ID: " << player.getId() << "\n";

  std::cout << "GameObject Name: " << enemy.getName() << "\n";
  std::cout << "GameObject ID: " << enemy.getId() << "\n";  
  if (player.removeComponent("RenderComponent"))
  {
    std::cout << "Component removed successfully\n";
  }
  else 
  {
    std::cout << "Removing component failed\n";
  }

  if (player.removeComponent("PhysicsComponent"))
  {
    std::cout << "Component removed successfully\n";
  }
  else 
  {
    std::cout << "Removing component failed\n";
  }

  return 0;
}
