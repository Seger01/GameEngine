#include "SaveArray.hpp"
#include "SaveGame.hpp"
#include "SaveGameTest.hpp"
#include <iostream>

void test1() {
  SaveGame sg{"code/output/newSave.json"};
  sg.addField("myField", "myValue");
  sg.setField("myField", "newValue");
  sg.addArray("myArray");
  SaveArray array{sg.getArray("myArray")};
  array.addField("myArrayField", "myArrayValue");
  sg.setArray("myArray", array);
  sg.store();
}

int main() { test1(); }
