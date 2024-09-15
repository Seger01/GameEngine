#include <iostream>
#include "SaveGame.hpp"

void readTest(SaveGame saveGame) {
	std::cout << "Level number: " << saveGame.getLevelNr() << std::endl;
	std::vector<unsigned> inventory = saveGame.getInventory();
	std::cout << "Inventory items:" << std::endl;
	for (unsigned item : inventory) {
		std::cout << item << std::endl;
	}
}

int main() {
	SaveGame saveGame("saveFile.json");
	readTest(saveGame);
}
