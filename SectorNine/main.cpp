#include <iostream>
#include <memory>

#include "GameEngine.hpp"

using namespace CW;

int main()
{

  /*std::unique_ptr<GameEngine> gameEngine = std::make_unique<GameEngine>();
  
  gameEngine->init();

  while (true)
  {
    char choice = gameEngine->handleInput();
    if (choice == 0)
      break;

    gameEngine->update();
    gameEngine->render();
  }

  gameEngine->release();*/
  Map dungeon;
  dungeon.generate();

  std::cout << "=== BSP Dungeon Map View ===\n";
  dungeon.print();
  return 0;
}
