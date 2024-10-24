#include "SaveArray.hpp"
#include "SaveGame.hpp"
#include "SaveGameManager.hpp"
#include <iostream>

void test1() {
  SaveGame sg{"code/output/newSave.json"};
  sg.addStringField("myField", "myValue");
  sg.setStringField("myField", "newValue");
  sg.addArray("myArray");
  SaveArray array{sg.getArray("myArray")};
  array.addStringField("myArrayField", "myArrayValue");
  sg.setArray("myArray", array);
  sg.store();
}

void test2() {
  // Create the manager
  SaveGameManager sgm;
  // Create a new save game in the manager
  SaveGame sg1 = sgm.createSaveGame("save1", "code/output/save1.json");
  // Add a field to the save game
  sg1.addStringField("myField", "myValue");
  // Add an array to the save game and retrieve it
  sg1.addArray("myArray");
  SaveArray sga = sg1.getArray("myArray");
  // Add a field to the array and set it back to the save game
  sga.addStringField("myArrayField", "myArrayValue");
  sg1.setArray("myArray", sga);
  // Store the save game
  sg1.store();
  sgm.deleteSaveGame("save1");
}

int main() { test2(); }
